/*
 * =============================================================================
 *
 *       Filename:  HttpResponse.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  11/14/2012 11:32:19 AM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <muduo/net/http/HttpResponse.h>
#include <muduo/net/Buffer.h>

#include <stdio.h>

using namespace muduo;
using namespace muduo::net;

void HttpResponse::appendToBuffer(Buffer* output) const 
{
	char buf[32];
	snprintf(buf, sizeof buf, "HTTP/1.1 %d", statusCode_);
	output->append(buf);
	output->append(statusMessage_);
	output->append("\r\n");

	if(closeConnection_)
	{
		output->append("Connection: close\r\n");
	}
	else 
	{
		snprintf(buf, sizeof buf, "Content-Length: %zd\r\n", body_.size());
		output->append(buf);
	}

	for(std::map<string, string>::const_iterator it = headers_.begin();
			it != headers_.end();
			++it)
	{
		output->append(it->first);
		output->append(": ");
		output->append(it->second);
		output->append("\r\n");
	}

	output->append("\r\n");
	output->append(body_);
}

