/*
 * =============================================================================
 *
 *       Filename:  TimerQueue.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/22/2012 08:08:40 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_NET_TIMERQUEUE_H
#define MUDUO_NET_TIMERQUEUE_H

#include <set>
#include <vector>

#include <boost/noncopyable.hpp>

#include <muduo/base/Mutex.h>
#include <muduo/base/Timestamp.h>
#include <muduo/net/Callbacks.h>
#include <muduo/net/Channel.h>

namespace muduo 
{
namespace net 
{
class EventLoop;
class Timer;
class TimerId;

class TimerQueue : boost::noncopyable 
{
	public :
		TimerQueue(EventLoop* loop);
		~TimerQueue();

		TimerId addTimer(const TimerCallback& cb,
				Timestamp when,
				double interval);

	private :
		typedef std::pair<Timestamp, Timer*> Entry;
		typedef std::set<Entry> TimerList;

		void scheduleInLoop(Timer* time);

		void handleRead();

		std::vector<Entry> getExpired(Timestamp now);
		void reset(const std::vector<Entry>& expired, Timestamp now);

		bool insert(Timer* time);
		EventLoop* loop_;
		const int timerfd_;
		Channel timerfdChannel_;
		TimerList timers_;
};
}
}
#endif
