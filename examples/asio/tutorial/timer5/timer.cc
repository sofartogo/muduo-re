/*
 * =============================================================================
 *
 *       Filename:  timer.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  11/19/2012 02:28:00 PM
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

#include <iostream>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>

class Printer : boost::noncopyable
{
	public :
		Printer(muduo::net::EventLoop* loop1, muduo::net::EventLoop* loop2)
			: loop1_(loop1),
			  loop2_(loop2),
			  count_()
		{
			loop1_->runAfter(1, boost::bind(&Printer::print1, this));
			loop2_->runAfter(1, boost::bind(&Printer::print2, this));
		}

		~Printer()
		{
			std::cout << "Final count is " << count_ << "\n";
		}

		void print1()
		{
			muduo::MutexLockGuard lock(mutex_);
			if(count_ < 10)
			{
				std::cout << "Timer 1: " << count_ << "\n";
				++count_;

				loop1_->runAfter(1, boost::bind(&Printer::print1, this));
			}
			else
			{
				loop1_->quit();
			}
		}

		void print2()
		{
			muduo::MutexLockGuard lock(mutex_);
			if(count_ < 10)
			{
				std::cout << "Timer 2: " << count_ << "\n";
				++count_;

				loop2_->runAfter(1, boost::bind(&Printer::print2, this));
			}
			else 
			{
				loop2_->quit();
			}
		}

	private :
		muduo::net::EventLoop* loop1_;
		muduo::net::EventLoop* loop2_;
		muduo::MutexLock mutex_;
		int count_;
};

int main()
{
	muduo::net::EventLoop loop;
	muduo::net::EventLoopThread loopThread;
	muduo::net::EventLoop*loopInAnotherThread = loopThread.startLoop();
	Printer printer(&loop, loopInAnotherThread);
	loop.loop();
}
