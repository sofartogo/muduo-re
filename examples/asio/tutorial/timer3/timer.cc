/*
 * =============================================================================
 *
 *       Filename:  timer.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  11/19/2012 02:12:50 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <muduo/net/EventLoop.h>

#include <iostream>
#include <boost/bind.hpp>

void print(muduo::net::EventLoop* loop, int* count)
{
	if(*count < 5)
	{
		std::cout << *count << "\n";
		++(*count);

		loop->runAfter(1, boost::bind(print, loop, count));
	}
	else 
	{
		loop->quit();
	}
}

int main()
{
	muduo::net::EventLoop loop;
	int count = 0;
	loop.runAfter(1, boost::bind(print, &loop, &count));
	loop.loop();
	std::cout << "Final count is " << count << "\n";
}
