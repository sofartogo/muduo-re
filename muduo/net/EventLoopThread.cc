/*
 * =============================================================================
 *
 *       Filename:  EventLoopThread.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  10/29/2012 05:07:12 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <muduo/net/EventLoopThread.h>

#include <muduo/net/EventLoop.h>

#include <boost/bind.hpp>

using namespace muduo;
using namespace muduo::net;

EventLoopThread::EventLoopThread()
	: loop_(NULL),
	  exiting_(false),
	  thread_(boost::bind(&EventLoopThread::threadFunc, this)),
	  mutex_(),
	  cond_(mutex_)
{}

EventLoopThread::~EventLoopThread()
{
	exiting_ = true;
	loop_->quit();
	thread_.join();
}

EventLoop* EventLoopThread::startLoop()
{
	assert(!thread_.started());
	thread_.start();

	{
		MutexLockGuard lock(mutex_);
		while(loop_ == NULL)
		{
			cond_.wait();
		}
	}
	return loop_;
}

void EventLoopThread::threadFunc()
{
	EventLoop loop;

	{
		MutexLockGuard lock(mutex_);
		loop_ = &loop;
		cond_.notify();
	}
	loop_->loop();

}
