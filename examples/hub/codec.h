/*
 * =============================================================================
 *
 *       Filename:  codec.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/19/2012 04:51:55 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_EXAMPLES_HUB_CODEC_H
#define MUDUO_EXAMPLES_HUB_CODEC_H

#include <muduo/base/Types.h>
#include <muduo/net/Buffer.h>

#include <boost/noncopyable.hpp>

namespace pubsub
{
using muduo::string;

enum ParseResult
{
	kError,
	kSuccess,
	kContinue,
};
ParseResult parseMessage(muduo::net::Buffer* buf,
		string* cmd,
		string* topic,
		string* content);
}

#endif
