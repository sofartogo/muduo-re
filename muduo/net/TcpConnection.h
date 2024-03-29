/*
 * =============================================================================
 *
 *       Filename:  TcpConnection.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/30/2012 02:59:38 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_NET_TCPCONNECTION_H
#define MUDUO_NET_TCPCONNECTION_H

#include <muduo/base/Mutex.h>
#include <muduo/base/StringPiece.h>
#include <muduo/base/Types.h>
#include <muduo/net/Callbacks.h>
#include <muduo/net/Buffer.h>
#include <muduo/net/InetAddress.h>

#include <boost/any.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

namespace muduo 
{
namespace net 
{

class Channel;
class EventLoop;
class Socket;

class TcpConnection : boost::noncopyable,
	public boost::enable_shared_from_this<TcpConnection>
{
	public :
		TcpConnection(EventLoop* loop,
			const string& name,
			int sockfd,
			const InetAddress& localAddr,
			const InetAddress& peerAddr);
		~TcpConnection();

		EventLoop* getLoop() const { return loop_; }
		const string& name() const { return name_; }
		const InetAddress& localAddress() const { return localAddr_; }
		const InetAddress& peerAddress() const { return peerAddr_; }
		bool connected() const { return state_ == kConnected; }

		void send(const void* message, size_t len);
		void send(const StringPiece& message);

		void send(Buffer* message);
		void shutdown();
		void setTcpNoDelay(bool on);

		void setContext(const boost::any& context)
		{ context_ = context; }

		boost::any& getContext() 
		{ return context_; }

		const boost::any& getContext() const 
		{ return context_; }

		void setConnectionCallback(const ConnectionCallback& cb)
		{ connectionCallback_ = cb; }

		void setMessageCallback(const MessageCallback& cb)
		{ messageCallback_ = cb; }

		void setWriteCompleteCallback(const WriteCompleteCallback& cb)
		{ writeCompleteCallback_ = cb; }

		Buffer* inputBuffer()
		{ return &inputBuffer_; }

		void setCloseCallback(const CloseCallback& cb)
		{ closeCallback_ = cb; }

		void connectEstablished();

		void connectDestroyed();

	private :
		enum StateE {kDisconnected, kConnecting, kConnected, kDisconnecting };
		void handleRead(Timestamp receiveTime);
		void handleWrite();
		void handleClose();
		void handleError();

		void sendInLoop(const StringPiece& message);
		void sendInLoop(const void* message, size_t len);
		void shutdownInLoop();
		void setState(StateE s) { state_ = s; }

		EventLoop* loop_;
		string name_;
		StateE state_;

		boost::scoped_ptr<Socket> socket_;
		boost::scoped_ptr<Channel> channel_;
		InetAddress localAddr_;
		InetAddress peerAddr_;
		ConnectionCallback connectionCallback_;
		MessageCallback messageCallback_;
		WriteCompleteCallback writeCompleteCallback_;
		CloseCallback closeCallback_;
		Buffer inputBuffer_;
		Buffer outputBuffer_;
		boost::any context_;
};

typedef boost::shared_ptr<TcpConnection> TcpConnectionPtr;

}
}

#endif
