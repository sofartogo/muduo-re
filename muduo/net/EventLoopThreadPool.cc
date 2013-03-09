/*
 * =============================================================================
 *
 *       Filename:  EventLoopThreadPool.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  10/29/2012 05:23:13 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <muduo/net/EventLoopThreadPool.h>

#include <muduo/net/EventLoop.h>
#include <muduo/net/EventLoopThread.h>

#include <boost/bind.hpp>

using namespace muduo;
using namespace muduo::net;

EventLoopThreadPool::EventLoopThreadPool(EventLoop* baseLoop)
	: baseLoop_(baseLoop),
	  started_(false),
	  numThreads_(0),
	  next_(0)
{}

EventLoopThreadPool::~EventLoopThreadPool()
{
	for(size_t i = 0; i < threads_.size(); i++)
	{
		delete threads_[i];
	}
}

void EventLoopThreadPool::start()
{
	assert(!started_);
	baseLoop_->assertInLoopThread();

	started_ = true;
	for(int i = 0; i < numThreads_; i ++)
	{
		EventLoopThread* t = new EventLoopThread;
		threads_.push_back(t);
		loops_.push_back(t->startLoop());
	}
}

EventLoop* EventLoopThreadPool::getNextLoop()
{
	baseLoop_->assertInLoopThread();
	EventLoop* loop = baseLoop_;
	if(!loops_.empty())
	{
		loop = loops_[next_];
		next_++;
		if (implicit_cast<size_t>(next_) >= loops_.size())
		{
			next_ = 0;
		}
	}
	return loop;
}


