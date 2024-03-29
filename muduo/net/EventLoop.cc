/*
 * =============================================================================
 *
 *       Filename:  EventLoop.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  10/22/2012 08:57:36 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <muduo/net/EventLoop.h>

#include <muduo/base/Logging.h>
#include <muduo/base/Mutex.h>
#include <muduo/base/Singleton.h>
#include <muduo/net/Channel.h>
#include <muduo/net/Poller.h>
#include <muduo/net/SocketsOps.h>
#include <muduo/net/TimerQueue.h>

#include <boost/bind.hpp>

#include <signal.h>
#include <sys/eventfd.h>

using namespace muduo;
using namespace muduo::net;

namespace
{
__thread EventLoop* t_loopInThisThread = 0;

const int kPollTimeMs = 10000;

int createEventfd()
{
	int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
	if(evtfd < 0)
	{
		LOG_SYSERR << "Failed in eventfd";
		abort();
	}

	return evtfd;
}

#pragma GCC diagnostic ignored "-Wold-style-cast"
class IgnoreSigPipe
{
	public :
		IgnoreSigPipe()
		{
			::signal(SIGPIPE, SIG_IGN);
			LOG_TRACE << "Ignore SIGPIPE";
		}
};

IgnoreSigPipe initObj;
}

EventLoop::EventLoop()
	: looping_(false),
	  quit_(false),
	  eventHandling_(false),
	  callingPendingFunctors_(false),
	  threadId_(CurrentThread::tid()),
	  poller_(Poller::newDefaultPoller(this)),
	  TimerQueue_(new TimerQueue(this)),
	  wakeupFd_(createEventfd()),
	  wakeupChannel_(new Channel(this, wakeupFd_))
{
	LOG_TRACE << "EventLoop created " << this << " in thread " << threadId_;
	if(t_loopInThisThread)
	{
		LOG_FATAL << " Another EventLoop " << t_loopInThisThread
			<< " exist in this thread " << threadId_;
	}
	else 
	{
		t_loopInThisThread = this;
	}
	wakeupChannel_->setReadCallback(
			boost::bind(&EventLoop::handleRead, this));

	wakeupChannel_->enableReading();
}

EventLoop::~EventLoop()
{
	::close(wakeupFd_);
	t_loopInThisThread = NULL;
}

void EventLoop::loop()
{
	assert(!looping_);
	assertInLoopThread();
	looping_ = true;
	quit_ = false;
	LOG_TRACE << "EventLoop " << this << " start looping";

	while (!quit_)
	{
		activeChannels_.clear();
		pollReturnTime_ = poller_->poll(kPollTimeMs, &activeChannels_);
		if(Logger::logLevel() <= Logger::TRACE)
		{
			printActiveChannels();
		}

		eventHandling_ = true;
		for(ChannelList::iterator it = activeChannels_.begin();
				it != activeChannels_.end(); it++)
		{
			(*it)->handleEvent(pollReturnTime_);
		}
		eventHandling_ = false;
		doPendingFunctors();
	}
	LOG_TRACE << "EventLoop " << this << " stop looping";
	looping_ = false;
}

void EventLoop::quit()
{
	quit_ = true;
	if(!isInLoopThread())
	{
		wakeup();
	}
}

void EventLoop::runInLoop(const Functor& cb)
{
	if(isInLoopThread())
	{
		cb();
	}
	else 
	{
		queueInLoop(cb);
		wakeup();
	}
}

void EventLoop::queueInLoop(const Functor& cb)
{
	{
		MutexLockGuard lock(mutex_);
		pendingFunctors_.push_back(cb);
	}

	if(isInLoopThread() && callingPendingFunctors_)
	{
		wakeup();
	}
}

TimerId EventLoop::runAt(const Timestamp& time, const TimerCallback& cb)
{
	return TimerQueue_->addTimer(cb, time, 0.0);
}

TimerId EventLoop::runAfter(double delay, const TimerCallback& cb)
{
	Timestamp time(addTime(Timestamp::now(), delay));
	return runAt(time, cb);
}

TimerId EventLoop::runEvery(double interval, const TimerCallback& cb)
{
	Timestamp time(addTime(Timestamp::now(), interval));
	return TimerQueue_->addTimer(cb, time, interval);
}

void EventLoop::updateChannel(Channel* channel)
{
	assert(channel->ownerLoop() == this);
	assertInLoopThread();
	poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel* channel)
{
	assert(channel->ownerLoop() == this);
	assertInLoopThread();
	poller_->removeChannel(channel);
}

void EventLoop::abortNotInLoopThread()
{
	LOG_FATAL << "EventLoop::abortNotInLoopThread - EventLoop " << this 
		<< " was created in threadId_ = " << threadId_
		<< ", current thread id = " << CurrentThread::tid();
}

void EventLoop::wakeup()
{
	uint64_t one = 1;
	ssize_t n = sockets::write(wakeupFd_, &one, sizeof one);
	if(n != sizeof one)
	{
		LOG_ERROR << "EventLoop::wakeup() writes " << n << " bytes instead of 8";
	}
}

void EventLoop::handleRead()
{
	uint64_t one = 1;
	ssize_t n = sockets::read(wakeupFd_, &one, sizeof one);
	if(n != sizeof one)
	{
		LOG_ERROR << "EventLoop::handleRead() read " << n << " bytes instead of 8";
	}
}

void EventLoop::doPendingFunctors()
{
	std::vector<Functor> functors;
	callingPendingFunctors_ = true;
	{
		MutexLockGuard lock(mutex_);
		functors.swap(pendingFunctors_);
	}

	for(size_t i = 0; i < functors.size(); i++)
	{
		functors[i]();
	}
	callingPendingFunctors_ = false;
}

void EventLoop::printActiveChannels() const 
{
	for(ChannelList::const_iterator it = activeChannels_.begin();
			it != activeChannels_.end(); it++)
	{
		const Channel* ch = *it;
		LOG_TRACE << "{" << ch->reventsToString() << "}";
	}
}
