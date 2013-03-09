/*
 * =============================================================================
 *
 *       Filename:  EventLoop_unittest.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  10/23/2012 10:25:24 AM
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
#include <muduo/base/Thread.h>
//#include <muduo/base/Logging.cc>

#include <boost/bind.hpp>

#include <stdio.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

EventLoop* g_loop;

void callback()
{
	printf("callback(): pid = %d, tid = %d\n", getpid(), CurrentThread::tid());
	EventLoop anotherLoop;
}

void threadFunc()
{
	printf("threadFunc(): pid = %d, tid = %d\n", getpid(), CurrentThread::tid());

	EventLoop loop;
	loop.runAfter(1.0, callback);
	loop.loop();
}

int main()
{
	//printf("logLevel = %d\n", muduo::Logger::logLevel());
	printf("main(): pid = %d, tid = %d\n", getpid(), CurrentThread::tid());

	EventLoop loop;
	Thread thread(threadFunc);
	thread.start();
	loop.loop();
}
