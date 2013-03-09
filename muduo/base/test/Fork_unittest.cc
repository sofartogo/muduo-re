/*
 * =============================================================================
 *
 *       Filename:  Fork_unittest.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  10/16/2012 03:39:17 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <muduo/base/Thread.h>

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

namespace 
{
__thread int x = 0;
}

void print()
{
	printf("pid = %d tid = %d x = %d\n", getpid(), muduo::CurrentThread::tid(), x);
}

int main()
{
	printf("parrent %d\n", getpid());
	print();
	x = 1;
	print();
	pid_t p = fork();
	if(p == 0)
	{
		printf("child %d\n", getpid());
		print();
		x = 2;
		print();

		if(fork() == 0)
		{
			printf("grandchild %d\n", getpid());
			print();
			x = 3;
			print();
		}
	}
	else 
	{
		print();
	}
}
