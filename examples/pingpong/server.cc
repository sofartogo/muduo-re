/*
 * =============================================================================
 *
 *       Filename:  server.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  11/21/2012 04:11:15 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <muduo/net/TcpServer.h>
#include <muduo/base/Logging.h>
#include <muduo/base/Atomic.h>
#include <muduo/base/Thread.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>

#include <boost/bind.hpp>

#include <utility>

#include <mcheck.h>
#include <stdio.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

void onConnection(const TcpConnectionPtr& conn)
{
	if(conn->connected())
	{
		conn->setTcpNoDelay(true);
	}
}

void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time)
{
	conn->send(buf);
}

int main(int argc, char* argv[])
{
	if(argc < 4)
	{
		fprintf(stderr, "Usage : server <address> <port> <threads>\n");
	}
	else 
	{
		LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();
		Logger::setLogLevel(Logger::WARN);

		const char* ip = argv[1];
		uint16_t port = static_cast<uint16_t>(atoi(argv[2]));
		InetAddress listenAddr(ip, port);
		int threadCount = atoi(argv[3]);

		EventLoop loop;

		TcpServer server(&loop, listenAddr, "PingPong");

		server.setConnectionCallback(onConnection);
		server.setMessageCallback(onMessage);

		if(threadCount > 1)
		{
			server.setThreadNum(threadCount);
		}

		server.start();

		loop.loop();
	}
}
