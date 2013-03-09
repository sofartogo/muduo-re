/*
 * =============================================================================
 *
 *       Filename:  main.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  11/20/2012 09:35:13 AM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include "echo.h"

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>

using namespace muduo;
using namespace muduo::net;

int main(int argc, char* argv[])
{
	LOG_INFO << "pid = " <<getpid();
	EventLoop loop;
	InetAddress listenAddr(2007); 
	int maxConnections = 5;
	if(argc > 1)
	{
		maxConnections = atoi(argv[1]);
	}
	LOG_INFO << "maxConnections = " << maxConnections;
	EchoServer server(&loop, listenAddr, maxConnections);
	server.start();
	loop.loop();
}
