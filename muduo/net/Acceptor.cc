/*
 * =============================================================================
 *
 *       Filename:  Acceptor.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  10/29/2012 03:34:30 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <muduo/net/Acceptor.h>

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/SocketsOps.h>

#include <boost/bind.hpp>

#include <errno.h>
#include <fcntl.h>

using namespace muduo;
using namespace muduo::net;

Acceptor::Acceptor(EventLoop* loop, const InetAddress& listenAddr)
	: loop_(loop),
	  acceptSocket_(sockets::createNonblockingOrDie()),
	  acceptChannel_(loop, acceptSocket_.fd()),
	  listenning_(false),
	  idleFd_(::open("/dev/null", O_RDONLY))
{
	acceptSocket_.setReuseAddr(true);
	acceptSocket_.bindAddress(listenAddr);
	acceptChannel_.setReadCallback(
			boost::bind(&Acceptor::handleRead, this));
}

Acceptor::~Acceptor()
{
	::close(idleFd_);
}

void Acceptor::listen()
{
	loop_->assertInLoopThread();
	listenning_ = true;
	acceptSocket_.listen();
	acceptChannel_.enableReading();
}

void Acceptor::handleRead()
{
	loop_->assertInLoopThread();
	InetAddress peerAddr(0);
	int connfd = acceptSocket_.accept(&peerAddr);
	if(connfd >= 0)
	{
		if(newConnectionCallback_)
		{
			newConnectionCallback_(connfd, peerAddr);
		}
		else 
		{
			sockets::close(connfd);
		}
	} 
	else 
	{
		if(errno == EMFILE)
		{
			::close(idleFd_);
			idleFd_ = ::accept(acceptSocket_.fd(), NULL, NULL);
			::close(idleFd_);
			idleFd_ = ::open("/dev/null", O_RDONLY);
		}
	}
}


