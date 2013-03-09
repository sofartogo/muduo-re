/*
 * =============================================================================
 *
 *       Filename:  TcpServer.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  10/30/2012 05:00:28 PM
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
#include <muduo/net/Acceptor.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/EventLoopThreadPool.h>
#include <muduo/net/SocketsOps.h>

#include <boost/bind.hpp>
#include <stdio.h>

using namespace muduo;
using namespace muduo::net;

TcpServer::TcpServer(EventLoop* loop,
		const InetAddress& listenAddr,
		const string& nameArg)
	: loop_(CHECK_NOTNULL(loop)),
	  hostport_(listenAddr.toHostPort()),
	  name_(nameArg),
	  acceptor_(new Acceptor(loop, listenAddr)),
	  threadPool_(new EventLoopThreadPool(loop)),
	  connectionCallback_(defaultConnectionCallback),
	  messageCallback_(defaultMessageCallback),
	  started_(false),
	  nextConnId_(1)
{
	acceptor_->setNewConnectionCallback(
			boost::bind(&TcpServer::newConnection, this, _1, _2));
}

TcpServer::~TcpServer()
{
	loop_->assertInLoopThread();
	LOG_TRACE << "TcpServer::~TcpServer [" << name_ << "] destructing";

	for(ConnectionMap::iterator it(connections_.begin());
			it != connections_.end(); it++)
	{
		TcpConnectionPtr conn = it->second;
		it->second.reset();
		conn->getLoop()->runInLoop(
				boost::bind(&TcpConnection::connectDestroyed, conn));
		conn.reset();
	}
}

void TcpServer::setThreadNum(int numThreads)
{
	assert(0 <= numThreads);
	threadPool_->setThreadNum(numThreads);
}

void TcpServer::start()
{
	if(!started_)
	{
		started_ = true;
		threadPool_->start();
	}

	if(!acceptor_->listenning())
	{
		loop_->runInLoop(
				boost::bind(&Acceptor::listen, get_pointer(acceptor_)));
	}
}

void TcpServer::newConnection(int sockfd, const InetAddress& peerAddr)
{
	loop_->assertInLoopThread();
	EventLoop* ioLoop = threadPool_->getNextLoop();
	char buf[32];
	snprintf(buf, sizeof buf, ":%s#%d", hostport_.c_str(), nextConnId_);
	++nextConnId_;
	string connName = name_ + buf;
	LOG_INFO << "TcpServer::newConnection [" << name_ 
		<< "] - new connection [" << connName 
		<< "] from " << peerAddr.toHostPort();
	InetAddress localAddr(sockets::getLocalAddr(sockfd));

	TcpConnectionPtr conn(
			new TcpConnection(ioLoop, connName, sockfd, localAddr, peerAddr));
	connections_[connName] = conn;
	conn->setConnectionCallback(connectionCallback_);
	conn->setMessageCallback(messageCallback_);
	conn->setWriteCompleteCallback(writeCompleteCallback_);
	conn->setCloseCallback(
			boost::bind(&TcpServer::removeConnection, this, _1));

	ioLoop->runInLoop(boost::bind(&TcpConnection::connectEstablished, conn));
}

void TcpServer::removeConnection(const TcpConnectionPtr& conn)
{
	loop_->runInLoop(boost::bind(&TcpServer::removeConnectionInLoop, this, conn));
}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr& conn)
{
	loop_->assertInLoopThread();
	LOG_INFO << "TcpServer::removeConnectionInLoop [" << name_ 
		<< "] - connection " << conn->name();
	size_t n = connections_.erase(conn->name());
	(void)n;

	assert(n == 1);
	EventLoop* ioLoop = conn->getLoop();
	ioLoop->queueInLoop(
			boost::bind(&TcpConnection::connectDestroyed, conn));
	if(!ioLoop->isInLoopThread())
	{
		ioLoop->wakeup();
	}
}
