/*
 * =============================================================================
 *
 *       Filename:  timer.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  11/19/2012 02:41:25 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <muduo/base/Mutex.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/EventLoopThread.h>

#include <stdio.h>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>


class Printer : boost::noncopyable
{
	public :
		Printer(muduo::net::EventLoop* loop1, muduo::net::EventLoop* loop2)
			: loop1_(loop1),
			  loop2_(loop2),
			  count_(0)
		{
			loop1_->runAfter(1, boost::bind(&Printer::print1, this));
			loop2_->runAfter(1, boost::bind(&Printer::print2, this));
		}

		~Printer()
		{
			printf("Final count is %d\n", count_);
		}

		void print1()
		{
			bool shouldQuit = false;
			int count = 0;

			{
				muduo::MutexLockGuard lock(mutex_);
				if(count_ < 10)
				{
					count = count_;
					++ count_;
				}
				else 
				{
					shouldQuit = true;
				}
			}
			if(shouldQuit)
			{
				loop1_->quit();
			}
			else 
			{
				printf("Timer 1: %d\n", count);
				loop1_->runAfter(1, boost::bind(&Printer::print1, this));
			}
		}

		void print2()
		{
			bool shouldQuit = false;
			int count = 0;
			{
				muduo::MutexLockGuard lock(mutex_);
				if(count_ < 10)
				{
					count = count_;
					count_ ++;
				}
				else 
				{
					shouldQuit = true;
				}
			}

			if(shouldQuit)
			{
				loop2_->quit();
			}
			else 
			{
				printf("Timer 2: %d\n", count);
				loop2_->runAfter(1, boost::bind(&Printer::print2, this));
			}
		}

	private :
		muduo::MutexLock mutex_;
		muduo::net::EventLoop* loop1_;
		muduo::net::EventLoop* loop2_;
		int count_;
};


int main()
{
	muduo::net::EventLoop loop;
	muduo::net::EventLoopThread loopThread;
	muduo::net::EventLoop* loopInAnotherThread = loopThread.startLoop();
	Printer printer(&loop, loopInAnotherThread);
	loop.loop();
}
