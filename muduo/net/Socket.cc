/*
 * =============================================================================
 *
 *       Filename:  Socket.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  10/17/2012 05:08:07 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <muduo/net/Socket.h>

#include <muduo/net/InetAddress.h>
#include <muduo/net/SocketsOps.h>

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <strings.h>

using namespace muduo;
using namespace muduo::net;

Socket::~Socket()
{
	sockets::close(sockfd_);
}

void Socket::bindAddress(const InetAddress& addr)
{
	sockets::bindOrDie(sockfd_, addr.getSockAddrInet());
}

void Socket::listen()
{
	sockets::listenOrDie(sockfd_);
}

int Socket::accept(InetAddress* peeraddr)
{
	struct sockaddr_in addr;
	bzero(&addr, sizeof addr);
	int connfd = sockets::accept(sockfd_, &addr);
	if(connfd >= 0)
	{
		peeraddr->setSockAddrInet(addr);
	}
	return connfd;
}

void Socket::shutdownWrite() 
{
	sockets::shutdownWrite(sockfd_);
}

void Socket::setTcpNoDelay(bool on)
{
	int optval = on ? 1 : 0;
	::setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY,
			&optval, sizeof optval);
}

void Socket::setReuseAddr(bool on)
{
	int optval = on ? 1 : 0;
	::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR,
			&optval, sizeof optval);
}
