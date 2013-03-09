/*
 * =============================================================================
 *
 *       Filename:  codec.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  11/19/2012 04:54:14 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include "codec.h"

using namespace muduo;
using namespace muduo::net;
using namespace pubsub;

ParseResult pubsub::parseMessage(Buffer* buf,
		string* cmd,
		string* topic,
		string* content)
{
	ParseResult result = kError;
	const char* crlf = buf->findCRLF();
	if(crlf)
	{
		const char* space = std::find(buf->peek(), crlf, ' ');
		if(space != crlf)
		{
			cmd->assign(buf->peek(), space);
			topic->assign(space+1, crlf);
			if(*cmd == "pub")
			{
				const char* start = crlf + 2;
				crlf = buf->findCRLF(start);
				if(crlf)
				{
					content->assign(start, crlf);
					buf->retrieveUntil(crlf+2);
					result = kSuccess;
				}
				else 
				{
					result = kContinue;
				}
			}
			else 
			{
				buf->retrieveUntil(crlf+2);
				result = kSuccess;
			}
		}
		else 
		{
			result = kError;
		}
	}
	else 
	{
		result = kContinue;
	}
	return result;
}


