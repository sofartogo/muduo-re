/*
 * =============================================================================
 *
 *       Filename:  Connector.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/29/2012 03:50:22 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_NET_CONNECTOR_H
#define MUDUO_NET_CONNECTOR_H

#include <muduo/net/InetAddress.h>

#include <boost/enable_shared_from_this.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

namespace muduo 
{
namespace net 
{

class Channel;
class EventLoop;

class Connector : boost::noncopyable,
	public boost::enable_shared_from_this<Connector>
{
	public :
		typedef boost::function<void (int sockfd)> NewConnectionCallback;
		Connector(EventLoop* loop, const InetAddress& serverAddr);
		~Connector();

		void setNewConnectionCallback(const NewConnectionCallback& cb)
		{ newConnectionCallback_ = cb; }

		void start();
		void restart();
		void stop();
		
		const InetAddress& serverAddress() const { return serverAddr_; }

	private :
		enum States { kDisconnected, kConnecting, kConnected };
		static const int kMaxRetryDelayMs = 30*1000;
		static const int kInitRetryDelayMs = 500;

		void setState(States s) { state_ = s; }
		void startInLoop();
		void connect();
		void connecting(int sockfd);
		void handleWrite();
		void handleError();
		void retry(int sockfd);
		int removeAndResetChannel();
		void resetChannel();

		EventLoop* loop_;
		InetAddress serverAddr_;
		bool connect_;
		States state_;
		boost::scoped_ptr<Channel> channel_;
		NewConnectionCallback newConnectionCallback_;
		int retryDelayMs_;
};
}
}
#endif
