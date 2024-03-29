/*
 * =============================================================================
 *
 *       Filename:  echo.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  11/20/2012 09:19:10 AM
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

#include <boost/bind.hpp>

using namespace muduo;
using namespace muduo::net;

EchoServer::EchoServer(EventLoop* loop,
		const InetAddress& listenAddr,
		int maxConnections)
	: loop_(loop),
	  server_(loop, listenAddr, "EchoServer"),
	  numConnected_(0),
	  kMaxConnections(maxConnections)
{
	server_.setConnectionCallback(
			boost::bind(&EchoServer::onConnection, this, _1));
	server_.setMessageCallback(
			boost::bind(&EchoServer::onMessage, this, _1, _2, _3));
}

void EchoServer::start()
{
	server_.start();
}

void EchoServer::onConnection(const TcpConnectionPtr& conn)
{
	LOG_INFO << "EchoServer - " << conn->peerAddress().toHostPort() << " -> "
		<< conn->localAddress().toHostPort() << " is "
		<< (conn->connected() ? "UP" : "DOWN");

	if(conn->connected())
	{
		++numConnected_;
		if(numConnected_ > kMaxConnections)
		{
			conn->shutdown();
		}
	}
	else 
	{
		--numConnected_;
	}
	LOG_INFO << "numConnected = " << numConnected_;
}

void EchoServer::onMessage(const TcpConnectionPtr& conn,
		Buffer* buf,
		Timestamp time)
{
	string msg(buf->retrieveAsString());
	LOG_INFO << conn->name() << " echo " << msg.size() << " bytes at " << time.toString();
	conn->send(msg);
}
