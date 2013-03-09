/*
 * =============================================================================
 *
 *       Filename:  HttpRequest.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/14/2012 11:05:41 AM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_NET_HTPP_HTTPREQUEST_H
#define MUDUO_NET_HTPP_HTTPREQUEST_H

#include <muduo/base/copyable.h>
#include <muduo/base/Timestamp.h>
#include <muduo/base/Types.h>

#include <map>
#include <assert.h>
#include <stdio.h>

namespace muduo 
{
namespace net
{

class HttpRequest : public muduo::copyable
{
	public :
		enum Method
		{
			kUnKnown, kGet, kPost, kHead, kPut, kDelete
		};

		HttpRequest()
			 : method_(kUnKnown)
		{}

		bool setMethod(const char* start, const char* end)
		{
			assert(method_ == kUnKnown);
			string m(start, end);
			if (m == "GET")
			{
				method_ = kGet;
			}
			else if (m == "POST")
			{
				method_ = kPost;
			}
			else if (m == "HEAD")
			{
				method_ = kHead;
			}
			else if (m == "PUT")
			{
				method_ = kPut;
			}
			else if (m == "DELETE")
			{
				method_ = kDelete;
			}
			else 
			{
				method_ = kUnKnown;
			}
			return method_ != kUnKnown;
		}

		Method method() const 
		{ return method_; }

		const char* methodString() const 
		{
			const char* result = "UNKNOWN";
			switch(method_)
			{
				case kGet:
					result = "GET";
					break;
				case kPost:
					result = "POST";
					break;
				case kHead:
					result = "HEAD";
					break;
				case kPut:
					result = "PUT";
					break;
				case kDelete:
					result = "DELETE";
					break;
				default:
					break;
			}
			return result;
		}

		void setPath(const char* start, const char* end)
		{
			path_.assign(start, end);
		}

		const string& path() const 
		{ return path_; }

		void setReceiveTime(Timestamp t)
		{ receiveTime_ = t; }

		Timestamp receiveTime() const 
		{ return receiveTime_; }

		void addHeader(const char* start, const char* colon, const char* end)
		{
			string field(start, colon);
			++colon;
			while(isspace(*colon))
			{
				++colon;
			}
			string value(colon, end);
			while(!value.empty() && isspace(value[value.size()-1]))
			{
				value.resize(value.size()-1);
			}
			headers_[field] = value;
		}

		string getHeader(const string& field) const
		{
			string result;
			std::map<string, string>::const_iterator it = headers_.find(field);
			if(it != headers_.end())
			{
				result = it->second;
			}
			return result;
		}

		const std::map<string, string>& headers() const 
		{ return headers_; }

		void swap(HttpRequest& that)
		{
			std::swap(method_, that.method_);
			path_.swap(that.path_);
			receiveTime_.swap(that.receiveTime_);
			headers_.swap(that.headers_);
		}

	private:
		Method method_;
		string path_;
		Timestamp receiveTime_;
		std::map<string, string> headers_;
};
}
}
#endif
