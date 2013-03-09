/*
 * =============================================================================
 *
 *       Filename:  Inspector_test.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  11/14/2012 04:47:23 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <muduo/net/inspect/Inspector.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/EventLoopThread.h>

using namespace muduo;
using namespace muduo::net;

int main()
{
	EventLoop loop;
	EventLoopThread t;
	Inspector ins(t.startLoop(), InetAddress(12345), "test");
	loop.loop();
}
