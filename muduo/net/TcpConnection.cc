/*
 * =============================================================================
 *
 *       Filename:  TcpConnection.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  10/30/2012 03:18:05 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <muduo/net/TcpConnection.h>

#include <muduo/base/Logging.h>
#include <muduo/net/Channel.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/Socket.h>
#include <muduo/net/SocketsOps.h>

#include <boost/bind.hpp>

#include <errno.h>
#include <stdio.h>

using namespace muduo;
using namespace muduo::net;

void muduo::net::defaultConnectionCallback(const TcpConnectionPtr& conn)
{
	LOG_TRACE << conn->localAddress().toHostPort() << " -> "
			  << conn->peerAddress().toHostPort() << " is "
			  << (conn->connected() ? "UP" : "DOWN");
}

void muduo::net::defaultMessageCallback(const TcpConnectionPtr&,
		Buffer* buf,
		Timestamp)
{
	buf->retrieveAll();
}

TcpConnection::TcpConnection(EventLoop* loop,
		const string& nameArg,
		int sockfd,
		const InetAddress& localAddr,
		const InetAddress& peerAddr)
	: loop_(CHECK_NOTNULL(loop)),
	  name_(nameArg),
	  state_(kConnecting),
	  socket_(new Socket(sockfd)),
	  channel_(new Channel(loop, sockfd)),
	  localAddr_(localAddr),
	  peerAddr_(peerAddr)
{
	channel_->setReadCallback(
			boost::bind(&TcpConnection::handleRead, this, _1));
	channel_->setWriteCallback(
			boost::bind(&TcpConnection::handleWrite, this));
	channel_->setCloseCallback(
			boost::bind(&TcpConnection::handleClose, this));
	channel_->setErrorCallback(
			boost::bind(&TcpConnection::handleError, this));
	LOG_DEBUG << "TcpConnection::ctor[" << name_ << "] at " << this 
		      << " fd = " << sockfd;
}

TcpConnection::~TcpConnection()
{
	LOG_DEBUG << "TcpConnection::dtor[" << name_ << "] at " << this 
		      << " fd = " << channel_->fd();
}

void TcpConnection::send(const void* data, size_t len)
{
	if(state_ == kConnected)
	{
		if(loop_->isInLoopThread())
		{
			sendInLoop(data, len);
		}
		else 
		{
			string message(static_cast<const char*>(data), len);
			loop_->runInLoop(
					boost::bind(&TcpConnection::sendInLoop,
						this,
						message));
		}
	}
}


void TcpConnection::send(const StringPiece& message)
{
	if(state_ == kConnected)
	{
		if(loop_->isInLoopThread())
		{
			sendInLoop(message);
		}
		else 
		{
			loop_->runInLoop(
					boost::bind(&TcpConnection::sendInLoop,
						this,
						message.as_string()));
		}
	}
}

void TcpConnection::send(Buffer* buf)
{
	if(state_ == kConnected)
	{
		if(loop_->isInLoopThread())
		{
			sendInLoop(buf->peek(), buf->readableBytes());
			buf->retrieveAll();
		}
		else 
		{
			loop_->runInLoop(
					boost::bind(&TcpConnection::sendInLoop,
						this,
						buf->retrieveAsString()));
		}
	}
}


void TcpConnection::sendInLoop(const StringPiece& message)
{
	sendInLoop(message.data(), message.size());
}

void TcpConnection::sendInLoop(const void* data, size_t len)
{
	loop_->assertInLoopThread();
	ssize_t nwrote = 0;
	if(!channel_->isWriting() && outputBuffer_.readableBytes() == 0)
	{
		nwrote = sockets::write(channel_->fd(), data, len);
		if(nwrote >= 0)
		{
			if(implicit_cast<size_t>(nwrote) < len)
			{
				LOG_TRACE << "I am going to write more data";
			}
			else if( writeCompleteCallback_)
			{
				loop_->queueInLoop(boost::bind(writeCompleteCallback_, shared_from_this()));
			}
		}
		else 
		{
			nwrote = 0;
			if(errno != EWOULDBLOCK)
			{
				LOG_SYSERR << "TcpConnection::sendInLoop";
			}
		}
	}

	assert(nwrote >= 0);
	if(implicit_cast<size_t>(nwrote) < len)
	{
		outputBuffer_.append(static_cast<const char*>(data)+nwrote, len-nwrote);
		if(!channel_->isWriting())
		{
			channel_->enableWriting();
		}
	}
}

void TcpConnection::shutdown()
{
	if(state_ == kConnected)
	{
		setState(kDisconnecting);
		loop_->runInLoop(boost::bind(&TcpConnection::shutdownInLoop, this));
	}
}

void TcpConnection::shutdownInLoop()
{
	loop_->assertInLoopThread();
	if(!channel_->isWriting())
	{
		socket_->shutdownWrite();
	}
}

void TcpConnection::setTcpNoDelay(bool on)
{
	socket_->setTcpNoDelay(on);
}

void TcpConnection::connectEstablished()
{
	loop_->assertInLoopThread();
	assert(state_ == kConnecting);
	setState(kConnected);
	channel_->tie(shared_from_this());
	channel_->enableReading();

	connectionCallback_(shared_from_this());
}

void TcpConnection::connectDestroyed()
{
	loop_->assertInLoopThread();
	if(state_ == kConnected)
	{
		setState(kDisconnected);
		channel_->disableAll();

		connectionCallback_(shared_from_this());
	}
	loop_->removeChannel(get_pointer(channel_));
}

void TcpConnection::handleRead(Timestamp receiveTime)
{
	loop_->assertInLoopThread();
	int savedErrno = 0;
	ssize_t n = inputBuffer_.readFd(channel_->fd(), &savedErrno);
	if(n > 0)
	{
		messageCallback_(shared_from_this(), &inputBuffer_, receiveTime);
	}
	else if(n == 0)
	{
		handleClose();
	}
	else 
	{
		handleError();
	}
}

void TcpConnection::handleWrite()
{
	loop_->assertInLoopThread();
	if(channel_->isWriting())
	{
		ssize_t n = sockets::write(channel_->fd(),
				outputBuffer_.peek(),
				outputBuffer_.readableBytes());
		if(n > 0)
		{
			outputBuffer_.retrieve(n);
			if(outputBuffer_.readableBytes() == 0)
			{
				channel_->disableWriting();
				if(writeCompleteCallback_)
				{
					loop_->queueInLoop(boost::bind(writeCompleteCallback_, shared_from_this()));
				}
				if(state_ == kDisconnecting)
				{
					shutdownInLoop();
				}
			}
			else 
			{
				LOG_TRACE << "I am going to write more data";
			}
		}
		else 
		{
			LOG_SYSERR << "TcpConnection::handleWrite";
			abort();
		}
	}
	else 
	{
		LOG_TRACE << "Connection is down, no nore writing";
	}
}

void TcpConnection::handleClose()
{
	loop_->assertInLoopThread();
	LOG_TRACE << "TcpConnection::handleClose state = " << state_;
	assert(state_ == kConnected || state_ == kDisconnecting);

	setState(kDisconnected);
	channel_->disableAll();

	TcpConnectionPtr guardThis(shared_from_this());
	connectionCallback_(guardThis);
	closeCallback_(guardThis);
}

void TcpConnection::handleError()
{
	int err = sockets::getSocketError(channel_->fd());
	LOG_ERROR << "TcpConnection::handleError [" << name_ 
		      << "] - SO_ERROR = " << err << " " << strerror_tl(err);
}
