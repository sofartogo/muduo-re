/*
 * =============================================================================
 *
 *       Filename:  HttpServer.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/14/2012 11:03:57 AM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_NET_HTTP_HTTPSERVER_H
#define MUDUO_NET_HTTP_HTTPSERVER_H

#include <muduo/net/TcpServer.h>
#include <boost/noncopyable.hpp>

namespace muduo 
{
namespace net 
{
class HttpRequest;
class HttpResponse;

class HttpServer : boost::noncopyable
{
	public :
		typedef boost::function<void (const HttpRequest&,
				HttpResponse*)> HttpCallback;
		HttpServer(EventLoop* loop,
				const InetAddress& listenAddr,
				const string& name);
		~HttpServer();

		void start();

		void setHttpCallback(const HttpCallback& cb)
		{ httpCallback_ = cb; }

	private :
		void onConnection(const TcpConnectionPtr& conn);
		void onMessage(const TcpConnectionPtr& conn, 
				Buffer* buf,
				Timestamp receiveTime);
		void onRequest(const TcpConnectionPtr&, const HttpRequest&);

		TcpServer server_;
		HttpCallback httpCallback_;
};
}
}

#endif
