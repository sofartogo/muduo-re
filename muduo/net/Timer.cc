/*
 * =============================================================================
 *
 *       Filename:  Timer.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  10/19/2012 03:56:43 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <muduo/net/Timer.h>

using namespace muduo;
using namespace muduo::net;

AtomicInt64 Timer::s_numCreated_;

void Timer::restart(Timestamp now)
{
	if (repeat_)
	{
		expiration_ = addTime(now, interval_);
	}
	else 
	{
		expiration_ = Timestamp::invalid();
	}
}
