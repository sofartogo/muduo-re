/*
 * =============================================================================
 *
 *       Filename:  server.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  11/21/2012 11:20:56 AM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <muduo/net/TcpServer.h>

#include <muduo/base/Atomic.h>
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

int numThreads = 0;

class EchoServer
{
	public :
		EchoServer(EventLoop* loop, const InetAddress& listenAddr)
			: loop_(loop),
			  server_(loop, listenAddr, "EchoServer"),
			  oldCounter_(0),
			  startTime_(Timestamp::now())
		{
			server_.setConnectionCallback(
					boost::bind(&EchoServer::onConnection, this, _1));
			server_.setMessageCallback(
					boost::bind(&EchoServer::onMessage, this, _1, _2, _3));
			server_.setThreadNum(numThreads);
			loop->runEvery(3.0, boost::bind(&EchoServer::printThroughput, this));
		}

		void start()
		{
			LOG_INFO << "starting " << numThreads << " threads.";
			server_.start();
		}

	private :
		void onConnection(const TcpConnectionPtr& conn)
		{
			LOG_INFO << conn->peerAddress().toHostPort() << " -> "
				<< conn->localAddress().toHostPort() << " is "
				<< (conn->connected() ? "UP" : "DOWN");
		}

		void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp)
		{
			size_t len = buf->readableBytes();
			transferred_.addAndGet(len);
			receivedMessage_.incrementAndGet();
			conn->send(buf);
		}

		void printThroughput()
		{
			Timestamp endTime = Timestamp::now();
			int64_t newCounter = transferred_.get();
			int64_t bytes = newCounter - oldCounter_;
			int64_t msgs = receivedMessage_.getAndSet(0);
			double time = timeDifference(endTime, startTime_);

			printf("%4.3f MiB/s %4.3f Ki Msg/s %6.2f bytes per msg\n",
					static_cast<double>(bytes)/time/1024/1024,
					static_cast<double>(msgs)/time/1024,
					static_cast<double>(bytes)/static_cast<double>(msgs));

			oldCounter_ = newCounter;
			startTime_ = endTime;
		}

		EventLoop* loop_;
		TcpServer server_;
		AtomicInt64 transferred_;
		AtomicInt64 receivedMessage_;
		int64_t oldCounter_;
		Timestamp startTime_;
};

int main(int argc, char* argv[])
{
	mtrace();
	LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();

	if(argc > 1)
	{
		numThreads = atoi(argv[1]);
	}

	EventLoop loop;
	InetAddress listenAddr(2007);
	EchoServer server(&loop, listenAddr);

	server.start();

	loop.loop();
}
