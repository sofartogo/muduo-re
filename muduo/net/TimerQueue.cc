/*
 * =============================================================================
 *
 *       Filename:  TimerQueue.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  10/22/2012 08:14:50 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#define __STDC_LIMIT_MACROS
#include <muduo/net/TimerQueue.h>

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/Timer.h>
#include <muduo/net/TimerId.h>

#include <boost/bind.hpp>

#include <sys/timerfd.h>

namespace muduo 
{

namespace net 
{
namespace detail
{
int createTimerfd()
{
	int timerfd = ::timerfd_create(CLOCK_MONOTONIC,
			TFD_NONBLOCK | TFD_CLOEXEC);
	if(timerfd < 0) 
	{
		LOG_SYSFATAL << "Failed in timerfd_create";
	}
	return timerfd;
}

struct timespec howMuchTimeFromNow(Timestamp when)
{
	int64_t microseconds = when.microSecondsSinceEpoch() - Timestamp::now().microSecondsSinceEpoch();

	if(microseconds < 100)
	{
		microseconds = 100;
	}

	struct timespec ts;
	ts.tv_sec = static_cast<time_t>(microseconds / Timestamp::kMicroSecondsPerSecond);
	ts.tv_nsec = static_cast<long>((microseconds % Timestamp::kMicroSecondsPerSecond) * 1000);
	return ts;
}

void readTimerfd(int timerfd, Timestamp now)
{
	uint64_t howmany;
	ssize_t n = ::read(timerfd, &howmany, sizeof howmany);
	if(n != sizeof howmany)
	{
		LOG_TRACE << "TimerQueue::handlerRead() " << howmany << " at " << now.toString();
	}
}

void resetTimerfd(int timerfd, Timestamp expiration)
{
	struct itimerspec newValue;
	struct itimerspec oldValue;
	bzero(&newValue, sizeof newValue);
	bzero(&oldValue, sizeof oldValue);
	newValue.it_value = howMuchTimeFromNow(expiration);
	int ret = ::timerfd_settime(timerfd, 0, &newValue, &oldValue);
	if(ret)
	{
		LOG_SYSERR << "timerfd_settime()";
	}
}

}
}
}


using namespace muduo;
using namespace muduo::net;
using namespace muduo::net::detail;

TimerQueue::TimerQueue(EventLoop* loop)
	: loop_(loop),
	  timerfd_(createTimerfd()),
	  timerfdChannel_(loop, timerfd_),
	  timers_()
{
	timerfdChannel_.setReadCallback(boost::bind(&TimerQueue::handleRead, this));
	timerfdChannel_.enableReading();
}

TimerQueue::~TimerQueue()
{
	::close(timerfd_);
	for(TimerList::iterator it = timers_.begin();
			it != timers_.end(); it++)
	{
		delete it->second;
	}
}

TimerId TimerQueue::addTimer(const TimerCallback& cb,
		Timestamp when,
		double interval)
{
	Timer* timer = new Timer(cb, when, interval);
	loop_->runInLoop(boost::bind(&TimerQueue::scheduleInLoop, this, timer));
	return TimerId(timer, timer->sequence());
}

void TimerQueue::scheduleInLoop(Timer* timer)
{
	loop_->assertInLoopThread();
	bool earliestChanged = insert(timer);

	if(earliestChanged)
	{
		resetTimerfd(timerfd_, timer->expiration());
	}
}

void TimerQueue::handleRead()
{
	loop_->assertInLoopThread();
	Timestamp now(Timestamp::now());
	readTimerfd(timerfd_, now);

	std::vector<Entry> expired = getExpired(now);

	for(std::vector<Entry>::iterator it = expired.begin();
			it != expired.end(); it++)
	{
		it->second->run();
	}
	reset(expired, now);
}

std::vector<TimerQueue::Entry> TimerQueue::getExpired(Timestamp now)
{
	std::vector<Entry> expired;
	Entry sentry = std::make_pair(now, reinterpret_cast<Timer*>(UINTPTR_MAX));
	TimerList::iterator it = timers_.lower_bound(sentry);
	assert(it == timers_.end() || now < it->first);
	std::copy(timers_.begin(), it, back_inserter(expired));
	timers_.erase(timers_.begin(), it);

	return expired;
}

void TimerQueue::reset(const std::vector<Entry>& expired, Timestamp now)
{
	Timestamp nextExpire;
	for(std::vector<Entry>::const_iterator it = expired.begin();
			it != expired.end(); it++)
	{
		if(it->second->repeat())
		{
			it->second->restart(now);
			insert(it->second);
		}
		else 
		{
			delete it->second;
		}
	}

	if(!timers_.empty())
	{
		nextExpire = timers_.begin()->second->expiration();
	}

	if(nextExpire.valid())
	{
		resetTimerfd(timerfd_, nextExpire);
	}
}

bool TimerQueue::insert(Timer* timer)
{
	bool earliestChanged = false;
	Timestamp when = timer->expiration();
	TimerList::iterator it = timers_.begin();
	if(it == timers_.end() || when < it->first)
	{
		earliestChanged = true;
	}
	std::pair<TimerList::iterator, bool> result = timers_.insert(std::make_pair(when, timer));
	assert(result.second);
	(void)result;
	return earliestChanged;
}
