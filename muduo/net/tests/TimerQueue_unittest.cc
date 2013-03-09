/*
 * =============================================================================
 *
 *       Filename:  TimerQueue_unittest.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  11/01/2012 10:24:48 AM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <muduo/net/EventLoop.h>
#include <muduo/net/EventLoopThread.h>
#include <muduo/base/Thread.h>

#include <boost/bind.hpp>

#include <stdio.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

EventLoop* g_loop;
int cnt = 0;

void printTid()
{
	printf("pid = %d, tid = %d\n", getpid(), CurrentThread::tid());
	printf("now %s\n", Timestamp::now().toString().c_str());
}

void print(const char* msg)
{
	printf("msg %s %s\n", Timestamp::now().toString().c_str(), msg);
	if(++cnt == 20)
	{
		g_loop->quit();
	}
}


int main()
{
	printTid();
	sleep(1);
	{
		EventLoop loop;
		g_loop = &loop;

		printf("main");
		loop.runAfter(1, boost::bind(print, "once1"));
		loop.runAfter(1.5, boost::bind(print, "once1.5"));
		loop.runAfter(2.5, boost::bind(print, "once2.5"));
		loop.runAfter(3.5, boost::bind(print, "once3.5"));
		loop.runEvery(2, boost::bind(print, "every2"));
		loop.runEvery(3, boost::bind(print, "every3"));

		loop.loop();
		print("main loop exist");
	}

	sleep(1);
	{
		EventLoopThread loopThread;
		EventLoop* loop = loopThread.startLoop();
		loop->runAfter(2, printTid);
		sleep(3);
		print("thread loop exist");
	}
}
