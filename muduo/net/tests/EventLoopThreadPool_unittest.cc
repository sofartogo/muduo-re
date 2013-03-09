/*
 * =============================================================================
 *
 *       Filename:  EventLoopThreadPool_unittest.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  10/31/2012 02:46:51 PM
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
#include <muduo/base/Thread.h>

#include <boost/bind.hpp>

#include <stdio.h>

using namespace muduo;
using namespace muduo::net;


void print()
{
	printf("main(): pid = %d, tid = %d\n", getpid(), CurrentThread::tid());
}

int main()
{
	print();

	EventLoop loop;
	loop.runAfter(11, boost::bind(&EventLoop::quit, &loop));

	{
		printf("Single thread:\n");
		EventLoopThreadPool model(&loop);
		model.setThreadNum(0);
		model.start();
		assert(model.getNextLoop() == &loop);
		assert(model.getNextLoop() == &loop);
		assert(model.getNextLoop() == &loop);
	}

	{
		printf("Another thread:\n");
		EventLoopThreadPool model(&loop);
		model.setThreadNum(1);
		model.start();
		EventLoop* nextLoop = model.getNextLoop();
		(void)nextLoop;
		nextLoop->runAfter(2, print);
		assert(nextLoop != &loop);
		assert(nextLoop == model.getNextLoop());
		assert(nextLoop == model.getNextLoop());
		::sleep(3);
	}

	{
		printf("Three threads:\n");
		EventLoopThreadPool model(&loop);
		model.setThreadNum(3);
		model.start();
		EventLoop* nextLoop = model.getNextLoop();
		(void)nextLoop;
		assert(nextLoop != &loop);
		assert(nextLoop != model.getNextLoop());
		assert(nextLoop != model.getNextLoop());
		assert(nextLoop == model.getNextLoop());
	}
	loop.loop();
}
