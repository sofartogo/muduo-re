/*
 * =============================================================================
 *
 *       Filename:  TimerId.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/19/2012 05:52:24 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_NET_TIMERID_H
#define MUDUO_NET_TIMERID_H

#include <muduo/base/copyable.h>

namespace muduo 
{
namespace net 
{

class Timer;

class TimerId : public muduo::copyable
{
	public :
		TimerId(Timer* timer, int64_t seq)
			: value_(timer),
			seq_(seq)
		{}

	private :
		Timer* value_;
		int64_t seq_;
};
}
}

#endif
