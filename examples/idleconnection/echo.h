/*
 * =============================================================================
 *
 *       Filename:  echo.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/19/2012 09:09:32 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_EXAMPLES_SIMPLE_ECHO_ECHO_H
#define MUDUO_EXAMPLES_SIMPLE_ECHO_ECHO_H

#include <muduo/net/TcpServer.h>

#include <boost/circular_buffer.hpp>
#include <boost/unordered_set.hpp>

class EchoServer
{
	public :
		EchoServer(muduo::net::EventLoop* loop,
				const muduo::net::InetAddress& listenAddr,
				int idleSeconds);

		void start();

	private :
		void onConnection(const muduo::net::TcpConnectionPtr& conn);

		void onMessage(const muduo::net::TcpConnectionPtr& conn,
				muduo::net::Buffer* buf,
				muduo::Timestamp time);

		void onTimer();

		void dumpConnectionBuckets() const;

		typedef boost::weak_ptr<muduo::net::TcpConnection> WeakTcpConnectionPtr;

		struct Entry : public muduo::copyable
		{
			Entry(const WeakTcpConnectionPtr& weakConn)
				: weakConn_(weakConn)
			{}

			~Entry()
			{
				muduo::net::TcpConnectionPtr conn = weakConn_.lock();
				if(conn)
				{
					conn->shutdown();
				}
			}
			WeakTcpConnectionPtr weakConn_;
		};

		typedef boost::shared_ptr<Entry> EntryPtr;
		typedef boost::weak_ptr<Entry> WeakEntryPtr;
		typedef boost::unordered_set<EntryPtr> Bucket;
		typedef boost::circular_buffer<Bucket> WeakConnectionList;

		muduo::net::EventLoop* loop_;
		muduo::net::TcpServer server_;
		WeakConnectionList connectionBuckets_;
};
#endif
