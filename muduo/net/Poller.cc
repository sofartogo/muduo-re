/*
 * =============================================================================
 *
 *       Filename:  Poller.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  10/22/2012 08:06:49 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <muduo/net/Poller.h>

using namespace muduo;
using namespace muduo::net;

Poller::Poller(EventLoop* loop)
	: ownerLoop_(loop)
{}

Poller::~Poller()
{}
