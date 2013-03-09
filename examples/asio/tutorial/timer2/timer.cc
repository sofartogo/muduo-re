/*
 * =============================================================================
 *
 *       Filename:  timer.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  11/19/2012 02:04:13 PM
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

void print()
{
	std::cout << "Hello, world!\n";
}

int main()
{
	muduo::net::EventLoop loop;
	loop.runAfter(5, print);
	loop.loop();
}
