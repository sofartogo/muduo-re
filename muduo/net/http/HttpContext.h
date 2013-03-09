/*
 * =============================================================================
 *
 *       Filename:  HttpContext.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/14/2012 11:42:27 AM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_NET_HTTP_HTTPCONTEXT_H
#define MUDUO_NET_HTTP_HTTPCONTEXT_H

#include <muduo/base/copyable.h>
#include <muduo/net/http/HttpRequest.h>

namespace muduo 
{
namespace net 
{
class HttpContext : public muduo::copyable 
{
	public :
		enum HttpRequestParseStatus
		{
			kExpectRequestLine,
			kExpectHeaders,
			kExpectBody,
			kGotAll,
		};

		HttpContext()
			: state_(kExpectRequestLine)
		{}

		bool expectRequestLine() const 
		{ return state_ == kExpectRequestLine; }

		bool expectHeaders() const 
		{ return state_ == kExpectHeaders; }

		bool expectBody() const 
		{ return state_ == kExpectBody; }

		bool gotAll() const 
		{ return state_ == kGotAll; }

		void receiveRequestLine()
		{ state_ = kExpectHeaders; }

		void receiveHeaders()
		{ state_ = kGotAll; }

		void reset()
		{
			state_ = kExpectRequestLine;
			HttpRequest dummy;
			request_.swap(dummy);
		}

		const HttpRequest& request() const 
		{ return request_; }

		HttpRequest& request()
		{ return request_; }

	private:
		HttpRequestParseStatus state_;
		HttpRequest request_;
};
}
}
#endif
