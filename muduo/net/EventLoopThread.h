/*
 * =============================================================================
 *
 *       Filename:  EventLoopThread.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/29/2012 05:04:29 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_NET_EVENTLOOPTHREAD_H
#define MUDUO_NET_EVENTLOOPTHREAD_H

#include <muduo/base/Condition.h>
#include <muduo/base/Mutex.h>
#include <muduo/base/Thread.h>

#include <boost/noncopyable.hpp>

namespace muduo  
{
namespace net 
{
class EventLoop;
class EventLoopThread : boost::noncopyable 
{
	public :
		EventLoopThread();
		~EventLoopThread();
		EventLoop* startLoop();

	private:
		void threadFunc();
		EventLoop* loop_;
		bool exiting_;
		Thread thread_;
		MutexLock mutex_;
		Condition cond_;
};
}
}

#endif
