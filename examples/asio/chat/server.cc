/*
 * =============================================================================
 *
 *       Filename:  server.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  11/19/2012 09:57:14 AM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include "codec.h"

#include <muduo/base/Logging.h>
#include <muduo/base/Mutex.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/SocketsOps.h>
#include <muduo/net/TcpServer.h>

#include <boost/bind.hpp>

#include <set>
#include <stdio.h>

using namespace muduo;
using namespace muduo::net;

class ChatServer : boost::noncopyable
{
	public :
		ChatServer(EventLoop* loop,
				const InetAddress& listenAddr)
			: loop_(loop),
			  server_(loop, listenAddr, "ChatServer"),
			  codec_(boost::bind(&ChatServer::onStringMessage, this, _1, _2, _3))
		{
			server_.setConnectionCallback(
					boost::bind(&ChatServer::onConnection, this, _1));
			server_.setMessageCallback(
					boost::bind(&LengthHeaderCodec::onMessage, &codec_, _1, _2, _3));
		}

		void start()
		{
			server_.start();
		}

	private :
		void onConnection(const TcpConnectionPtr& conn)
		{
			LOG_INFO << conn->localAddress().toHostPort() << " -> "
				<< conn->peerAddress().toHostPort() << " is "
				<< (conn->connected() ? "UP" : "DOWN");

			MutexLockGuard lock(mutex_);
			if(conn->connected())
			{
				connections_.insert(conn);
			}
			else 
			{
				connections_.erase(conn);
			}
		}

		void onStringMessage(const TcpConnectionPtr&,
				const string& message,
				Timestamp)
		{
			MutexLockGuard lock(mutex_);
			for(connectionList::iterator it = connections_.begin();
					it != connections_.end();
					it++)
			{
				codec_.send(get_pointer(*it), message);
			}
		}

		typedef std::set<TcpConnectionPtr> connectionList;
		EventLoop* loop_;
		TcpServer server_;
		LengthHeaderCodec codec_;
		MutexLock mutex_;
		connectionList connections_;
};


int main(int argc, char* argv[])
{
	LOG_INFO << "pid = " << getpid();
	if(argc > 1)
	{
		EventLoop loop;
		uint16_t port = static_cast<uint16_t>(atoi(argv[1]));
		InetAddress serverAddr(port);
		ChatServer server(&loop, serverAddr);
		server.start();
		loop.loop();
	}
	else 
	{
		printf("Usage: %s port\n", argv[0]);
	}
}
