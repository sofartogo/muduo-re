/*
 * =============================================================================
 *
 *       Filename:  TcpServer.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/30/2012 04:53:21 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_NET_TCPSERVER_H
#define MUDUO_NET_TCPSERVER_H

#include <muduo/base/Types.h>
#include <muduo/net/Callbacks.h>
#include <muduo/net/TcpConnection.h>

#include <map>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

namespace muduo 
{
namespace net 
{
class Acceptor;
class EventLoop;
class EventLoopThreadPool;


class TcpServer : boost::noncopyable 
{
	public :
		TcpServer(EventLoop* loop,
				const InetAddress& listenAddr,
				const string& nameArg);
		~TcpServer();

		const string& hostport() const { return hostport_; }
		const string& name() const { return name_; }

		void setThreadNum(int numThreads);

		void start();
		void setConnectionCallback(const ConnectionCallback& cb)
		{ connectionCallback_ = cb; }

		void setMessageCallback(const MessageCallback& cb)
		{ messageCallback_ = cb; }

		void setWriteCompleteCallback(const WriteCompleteCallback& cb)
		{ writeCompleteCallback_ = cb; }

	private :
		void newConnection(int sockfd, const InetAddress& peerAddr);

		void removeConnection(const TcpConnectionPtr& conn);

		void removeConnectionInLoop(const TcpConnectionPtr& conn);

		typedef std::map<string, TcpConnectionPtr> ConnectionMap;

		EventLoop* loop_;
		const string hostport_;
		const string name_;
		boost::scoped_ptr<Acceptor> acceptor_;
		boost::scoped_ptr<EventLoopThreadPool> threadPool_;
		ConnectionCallback connectionCallback_;
		MessageCallback messageCallback_;
		WriteCompleteCallback writeCompleteCallback_;

		bool started_;

		int nextConnId_;
		ConnectionMap connections_;
};
}
}
#endif
