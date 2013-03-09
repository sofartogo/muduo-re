/*
 * =============================================================================
 *
 *       Filename:  ThreadPool_test.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  10/17/2012 11:28:51 AM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <muduo/base/ThreadPool.h>
#include <muduo/base/CountDownLatch.h>

#include <boost/bind.hpp>
#include <stdio.h>

void print()
{
	printf("tid = %d\n", muduo::CurrentThread::tid());
}

void printString(const std::string& str)
{
	printf("tid = %d, str = %s\n", muduo::CurrentThread::tid(), str.c_str());
}

int main()
{
	muduo::ThreadPool pool("MainThreadPool");
	pool.start(5);

	pool.run(print);
	pool.run(print);

	for(int i = 0; i < 100; i++)
	{
		char buf[32];
		snprintf(buf, sizeof buf, "task %d", i);
		pool.run(boost::bind(printString, std::string(buf)));
	}

	muduo::CountDownLatch latch(1);
	pool.run(boost::bind(&muduo::CountDownLatch::countDown, &latch));
	latch.wait();
	pool.stop();
}
