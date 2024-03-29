/*
 * =============================================================================
 *
 *       Filename:  codec.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/19/2012 09:57:32 AM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_EXAMPLES_ASIO_CHAT_CODEC_H
#define MUDUO_EXAMPLES_ASIO_CHAT_CODEC_H

#include <muduo/base/Logging.h>
#include <muduo/net/Buffer.h>
#include <muduo/net/Endian.h>
#include <muduo/net/TcpConnection.h>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

class LengthHeaderCodec : boost::noncopyable
{
	public :
		typedef boost::function<void (const muduo::net::TcpConnectionPtr&,
				const muduo::string& message,
				muduo::Timestamp)> StringMessageCallback;

		explicit LengthHeaderCodec(const StringMessageCallback& cb)
			: messageCallback_(cb)
		{}

		void onMessage(const muduo::net::TcpConnectionPtr& conn,
				muduo::net::Buffer* buf,
				muduo::Timestamp receiveTime)
		{
			while (buf->readableBytes() >= kHeaderLen)
			{
				const void* data = buf->peek();
				int32_t be32 = *static_cast<const int32_t*>(data);
				const int32_t len = muduo::net::sockets::networkToHost32(be32);
				if(len > 65535 || len < 0)
				{
					LOG_ERROR << "Invalid length " << len;
					conn->shutdown();
				}
				else if(buf->readableBytes() >= len + kHeaderLen)
				{
					buf->retrieve(kHeaderLen);
					muduo::string message(buf->peek(), len);
					buf->retrieve(len);
					messageCallback_(conn, message, receiveTime);
				}
				else 
				{
					break;
				}
			}
		}

		void send(muduo::net::TcpConnection* conn, const muduo::string& message)
		{
			muduo::net::Buffer buf;
			buf.append(message.data(), message.size());
			int32_t len = muduo::net::sockets::hostToNetwork32(static_cast<int32_t>(message.size()));
			buf.prepend(&len, sizeof len);
			conn->send(&buf);
		}

	private :
		StringMessageCallback messageCallback_;
		const static size_t kHeaderLen = sizeof(int32_t);
};

#endif

