/*
 * =============================================================================
 *
 *       Filename:  echo.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/20/2012 09:16:29 AM
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

class EchoServer
{
	public :
		EchoServer(muduo::net::EventLoop* loop,
				const muduo::net::InetAddress& listenAddr,
				int maxConnections);

		void start();

	private :
		void onConnection(const muduo::net::TcpConnectionPtr& conn);

		void onMessage(const muduo::net::TcpConnectionPtr& conn,
				muduo::net::Buffer* buf,
				muduo::Timestamp time);

		muduo::net::EventLoop* loop_;
		muduo::net::TcpServer server_;
		int numConnected_;
		const int kMaxConnections;
};

#endif
