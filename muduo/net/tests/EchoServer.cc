/*
 * =============================================================================
 *
 *       Filename:  EchoServer.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  11/01/2012 10:37:14 AM
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
#include <muduo/base/Thread.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>

#include <boost/bind.hpp>
#include <utility>

#include <mcheck.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

int numThreads = 0;

class EchoServer
{
	public :
		EchoServer(EventLoop* loop, const InetAddress& listenAddr)
			: loop_(loop),
			  server_(loop, listenAddr, "EchoServer")
		{
			server_.setConnectionCallback(
					boost::bind(&EchoServer::onConnection, this, _1));
			server_.setMessageCallback(
					boost::bind(&EchoServer::onMessage, this, _1, _2, _3));
			server_.setThreadNum(numThreads);
		}

		void start()
		{
			server_.start();
		}
	private :
		void onConnection(const TcpConnectionPtr& conn)
		{
			LOG_TRACE << conn->peerAddress().toHostPort() << " -> " 
				<< conn->localAddress().toHostPort() << " is "
				<< (conn->connected() ? "UP" : "DOWN");
			conn->send("hello\n");
		}

		void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time)
		{
			string msg(buf->retrieveAsString());
			LOG_TRACE << conn->name() << " recv " << msg.size() << " bytes at " << time.toString();
			if(msg == "exit\n")
			{
				conn->send("bye\n");
				conn->shutdown();
			}
			if(msg == "quit\n")	
			{
				loop_->quit();
			}
			conn->send(msg);
		}

		EventLoop* loop_;
		TcpServer server_;
};


int main(int argc, char* argv[])
{
	mtrace();
	LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();
	LOG_INFO << "sizeof TcpConnection = " << sizeof(TcpConnection);
	if(argc > 1)
	{
		numThreads = atoi(argv[1]);
	}

	EventLoop loop;
	InetAddress listenAddr(2000);
	EchoServer server(&loop, listenAddr);

	server.start();
	loop.loop();
}
