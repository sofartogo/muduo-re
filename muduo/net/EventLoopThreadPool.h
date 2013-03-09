/*
 * =============================================================================
 *
 *       Filename:  EventLoopThreadPool.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/29/2012 05:19:07 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_NET_EVENTLOOPTHREADPOOL_H
#define MUDUO_NET_EVENTLOOPTHREADPOOL_H

#include <muduo/base/Condition.h>
#include <muduo/base/Mutex.h>

#include <vector>
#include <boost/noncopyable.hpp>

namespace muduo 
{
class Thread;

namespace net 
{
class EventLoop;
class EventLoopThread;

class EventLoopThreadPool : boost::noncopyable 
{
	public :
		EventLoopThreadPool(EventLoop* baseLoop);
		~EventLoopThreadPool();

		void setThreadNum(int numThreads) { numThreads_ = numThreads; }
		void start();
		EventLoop* getNextLoop();
	
	private :
		EventLoop* baseLoop_;
		bool started_;
		int numThreads_;
		int next_;
		std::vector<EventLoopThread*> threads_;
		std::vector<EventLoop*> loops_;
};

}
}
#endif
