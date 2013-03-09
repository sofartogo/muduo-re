/*
 * =============================================================================
 *
 *       Filename:  EventLoop.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/22/2012 09:16:11 AM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_NET_EVENTLOOP_H
#define MUDUO_NET_EVENTLOOP_H

#include <vector>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include <muduo/base/Mutex.h>
#include <muduo/base/Thread.h>
#include <muduo/base/Timestamp.h>
#include <muduo/net/Callbacks.h>
#include <muduo/net/TimerId.h>

namespace muduo 
{
namespace net 
{
class Channel;
class Poller;
class TimerQueue;

class EventLoop : boost::noncopyable 
{
	public :
		typedef boost::function<void()> Functor;

		EventLoop();
		~EventLoop();

		void loop();
		void quit();

		Timestamp pollReturnTime() const { return pollReturnTime_; }

		void runInLoop(const Functor& cb);

		void queueInLoop(const Functor& cb);

		TimerId runAt(const Timestamp& time, const TimerCallback& cb);
		TimerId runAfter(double delay, const TimerCallback& cb);
		TimerId runEvery(double interval, const TimerCallback& cb);

		void wakeup();
		void updateChannel(Channel* channel);
		void removeChannel(Channel* channel);

		void assertInLoopThread()
		{
			if(!isInLoopThread())
			{
				abortNotInLoopThread();
			}
		}

		bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }
	
	private :
		void abortNotInLoopThread();
		void handleRead();
		void doPendingFunctors();
		
		void printActiveChannels() const;

		typedef std::vector<Channel*> ChannelList;

		bool looping_;
		bool quit_;
		bool eventHandling_;
		bool callingPendingFunctors_;

		const pid_t threadId_;
		Timestamp pollReturnTime_;
		boost::scoped_ptr<Poller> poller_;
		boost::scoped_ptr<TimerQueue> TimerQueue_;
		int wakeupFd_;

		boost::scoped_ptr<Channel> wakeupChannel_;
		ChannelList activeChannels_;
		MutexLock mutex_;
		std::vector<Functor> pendingFunctors_;
};
}
}
#endif
