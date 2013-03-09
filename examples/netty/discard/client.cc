/*
 * =============================================================================
 *
 *       Filename:  client.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  11/21/2012 10:58:52 AM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <muduo/net/TcpClient.h>
#include <muduo/base/Logging.h>
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

class DiscardClient : boost::noncopyable 
{
	public :
		DiscardClient(EventLoop* loop, const InetAddress& listenAddr, int size)
			: loop_(loop),
			  client_(loop, listenAddr, "DiscardClient"),
			  message_(size, 'H')
		{
			client_.setConnectionCallback(
					boost::bind(&DiscardClient::onConnection, this, _1));
			client_.setMessageCallback(
					boost::bind(&DiscardClient::onMessage, this, _1, _2, _3));
			client_.setWriteCompleteCallback(
					boost::bind(&DiscardClient::onWriteComplete, this, _1));
		}

		void connect()
		{
			client_.connect();
		}
	
	private:
		void onConnection(const TcpConnectionPtr& conn)
		{
			LOG_TRACE << conn->localAddress().toHostPort() << " -> "
				<< conn->peerAddress().toHostPort() << " is "
				<< (conn->connected() ? "UP" : "DOWN");

			if(conn->connected())
				conn->send(message_);
			else loop_->quit();
		}

		void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time)
		{
			buf->retrieveAll();
		}

		void onWriteComplete(const TcpConnectionPtr& conn)
		{
			conn->send(message_);
		}

		EventLoop* loop_;
		TcpClient client_;
		string message_;
};


int main(int argc, char* argv[])
{
	LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();
	if(argc > 1)
	{
		EventLoop loop;
		InetAddress serverAddr(argv[1], 2009);

		int size = 256;
		if(argc > 2)
		{
			size = atoi(argv[2]);
		}

		DiscardClient client(&loop, serverAddr, size);
		client.connect();
		loop.loop();
	}
	else 
	{
		printf("Usage: %s hostip [msg_sizae]\n", argv[0]);
	}
}
