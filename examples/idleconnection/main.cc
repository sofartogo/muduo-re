/*
 * =============================================================================
 *
 *       Filename:  main.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  11/19/2012 09:34:31 PM
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
	EventLoop loop;
	InetAddress listenAddr(2007);
	int idleSeconds = 10;
	if(argc > 1)
	{
		idleSeconds = atoi(argv[1]);
	}
	LOG_INFO << "pid = " << getpid() << ", idle seconds = " << idleSeconds;
	EchoServer server(&loop, listenAddr, idleSeconds);
	server.start();
	loop.loop();
}
