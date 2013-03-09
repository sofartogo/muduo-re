/*
 * =============================================================================
 *
 *       Filename:  Thread_bench.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  10/16/2012 03:48:29 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <muduo/base/Thread.h>
#include <muduo/base/Timestamp.h>

#include <string>
#include <boost/bind.hpp>
#include <stdio.h>
#include <sys/wait.h>

void threadFunc()
{
	//printf("tid=%d\n", muduo::CurrentThread::tid());
}

void forkBench()
{
	sleep(10);
	muduo::Timestamp start(muduo::Timestamp::now());
	int  kProcesses = 10*1000;

	for(int i = 0; i < kProcesses; i++)
	{
		pid_t child = fork();
		if(child == 0) {
			exit(0);	
		} else {
			waitpid(child, NULL, 0);
		}
	}

	double timeused = timeDifference(muduo::Timestamp::now(), start);
	printf("process creation time used %f us\n", timeused*1000000/kProcesses);
	printf("number of created processes %d\n", kProcesses);
}

int main(int argc, char* argv[])
{
	printf("pid = %d, tid = %d\n", ::getpid(), muduo::CurrentThread::tid());
	muduo::Timestamp start(muduo::Timestamp::now());

	int kProcesses = 100*1000;
	for(int i = 0; i < kProcesses; i++)
	{
		muduo::Thread t1(threadFunc);
		t1.start();
		t1.join();
	}

	double timeUsed = timeDifference(muduo::Timestamp::now(), start);
	printf("thread creation time %f us\n", timeUsed*1000000/kProcesses);
	printf("number of created threads %d\n", kProcesses);

	forkBench();
}
