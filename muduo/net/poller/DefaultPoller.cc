/*
 * =============================================================================
 *
 *       Filename:  DefaultPoller.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  10/23/2012 10:35:42 AM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <muduo/net/Poller.h>
#include <muduo/net/poller/PollPoller.h>
#include <muduo/net/poller/EPollPoller.h>

#include <stdlib.h>

using namespace muduo::net;

Poller* Poller::newDefaultPoller(EventLoop* loop)
{
	if(::getenv("MUDUO_USE_POLL"))
	{
		return new PollPoller(loop);
	}
	else 
	{
		return new EPollPoller(loop);
	}
}
