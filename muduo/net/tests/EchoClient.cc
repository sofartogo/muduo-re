/*
 * =============================================================================
 *
 *       Filename:  EchoClient.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  11/01/2012 11:33:38 AM
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

#include <mcheck.h>
#include <stdio.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

int numThreads = 0;
class EchoClient;
std::vector<EchoClient*> clients;
int current = 0;

class EchoClient : boost::noncopyable
{
	public :
		EchoClient(EventLoop* loop, InetAddress& listenAddr, const string& id)
			: loop_(loop),
			  client_(loop, listenAddr, "EchoClient" + id)
		{
			client_.setConnectionCallback(
					boost::bind(&EchoClient::onConnection, this, _1));
			client_.setMessageCallback(
					boost::bind(&EchoClient::onMessage, this, _1, _2, _3));
		}

		void connect()
		{
			client_.connect();
		}

	private :
		void onConnection(const TcpConnectionPtr& conn)
		{
			LOG_TRACE << conn->localAddress().toHostPort() << " -> "
				<< conn->peerAddress().toHostPort() << " is "
				<< (conn->connected() ? "UP" : "DOWN");

			if(conn->connected())
			{
				++ current;
				if(implicit_cast<size_t>(current) < clients.size())
				{
					clients[current]->connect();
				}
				LOG_INFO << "*** connected " << current;
			}
			conn->send("world\n");
		}

		void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time)
		{
			string msg(buf->retrieveAsString());
			LOG_TRACE << conn->name() << " recv " << msg.size() << " bytes at " << time.toString();
		}

		EventLoop* loop_;
		TcpClient client_;
};


int main(int argc, char* argv[])
{
	LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();
	if(argc > 1)
	{
		EventLoop loop;
		InetAddress serverAddr(argv[1], 2000);
		int n = 1;
		if(argc > 2)
		{
			n = atoi(argv[2]);
		}

		clients.reserve(n);
		for(int i = 0; i < n; i++)
		{
			char buf[32];
			snprintf(buf, sizeof buf, "%d", i+1);
			clients.push_back(new EchoClient(&loop, serverAddr, buf));
		}

		clients[current]->connect();
		loop.loop();
		while(!clients.empty())
		{
			delete clients.back();
			clients.pop_back();
		}
	}
	else 
	{
		printf("Usage: %s host_ip [current#]\n", argv[0]);
	}
}
