/*
 * =============================================================================
 *
 *       Filename:  HttpResponse.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/14/2012 11:24:20 AM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_NET_HTTP_HTTPRESPONSE_H
#define MUDUO_NET_HTTP_HTTPRESPONSE_H

#include <muduo/base/copyable.h>
#include <muduo/base/Types.h>

#include <map>

namespace muduo 
{
namespace net
{
class Buffer;
class HttpResponse : public muduo::copyable
{
	public :
		enum HttpStatusCode
		{
			kUnKnown,
			k200Ok = 200,
			k301MovedPermanently = 301,
			k400BadRequest = 400,
			k404NotFound = 404,
		};

		explicit HttpResponse(bool close)
			: statusCode_(kUnKnown),
			  closeConnection_(close)
		{}

		void setStatusCode(HttpStatusCode code)
		{ statusCode_ = code; }

		void setStatusMessage(const string& message)
		{ statusMessage_ = message; }

		void setCloseConnection(bool on)
		{ closeConnection_ = on; }

		bool closeConnection() const 
		{ return closeConnection_; }

		void setContentType(const string& contentType)
		{ addHeader("Content-Type", contentType); }

		void addHeader(const string& key, const string& value)
		{ headers_[key] = value; }

		void setBody(const string& body)
		{ body_ = body; }

		void appendToBuffer(Buffer* output) const;
	private :
		std::map<string, string> headers_;
		HttpStatusCode statusCode_;
		string statusMessage_;
		bool closeConnection_;
		string body_;
};
}
}

#endif
