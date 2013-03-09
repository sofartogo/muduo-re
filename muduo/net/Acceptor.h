/*
 * =============================================================================
 *
 *       Filename:  Acceptor.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/29/2012 03:26:16 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_NET_ACCEPTOR_H
#define MUDUO_NET_ACCEPTOR_H 

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

#include <muduo/net/Channel.h>
#include <muduo/net/Socket.h>

namespace muduo 
{
namespace net 
{
class EventLoop;
class InetAddress;


class Acceptor : boost::noncopyable 
{
	public :
		typedef boost::function<void (int sockfd, 
				const InetAddress&)> NewConnectionCallback;

		Acceptor(EventLoop* loop, const InetAddress& listenAddr);
		~Acceptor();

		void setNewConnectionCallback(const NewConnectionCallback& cb)
		{ newConnectionCallback_ = cb; }

		bool listenning() const { return listenning_; }
		void listen();
	
	private :
		void handleRead();

		EventLoop* loop_;
		Socket acceptSocket_;
		Channel acceptChannel_;
		NewConnectionCallback newConnectionCallback_;
		bool listenning_;
		int idleFd_;
};
}
}
#endif
