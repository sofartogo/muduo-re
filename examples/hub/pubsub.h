/*
 * =============================================================================
 *
 *       Filename:  pubsub.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/19/2012 06:00:26 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_EXAMPLES_HUB_PUBSUB_H
#define MUDUO_EXAMPLES_HUB_PUBSUB_H

#include <muduo/net/TcpClient.h>

namespace pubsub
{
using namespace muduo;
using namespace muduo::net;

class PubSubClient : boost::noncopyable
{
	public :
		typedef boost::function<void (PubSubClient*)> ConnectionCallback;
		typedef boost::function<void (const string& topic,
				const string& content,
				Timestamp)> SubscribeCallback;

		PubSubClient(muduo::net::EventLoop* loop,
				const muduo::net::InetAddress& hubAddr,
				const string& name);

		void start();
		void stop();
		bool connected() const;

		void setConnectionCallback(const ConnectionCallback& cb)
		{ connectionCallback_ = cb; }

		bool subscribe(const string& topic, const SubscribeCallback& cb);
		void unsubscribe(const string& topic);
		bool publish(const string& topic, const string& content);

	private :
		void onConnection(const muduo::net::TcpConnectionPtr& conn);
		void onMessage(const muduo::net::TcpConnectionPtr& conn,
				muduo::net::Buffer* buf,
				muduo::Timestamp receiveTime);
		bool send(const string& message);

		muduo::net::EventLoop* loop_;
		muduo::net::TcpClient client_;
		muduo::net::TcpConnectionPtr conn_;
		ConnectionCallback connectionCallback_;
		SubscribeCallback subscribeCallback_;
};
}

#endif
