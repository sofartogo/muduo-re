/*
 * =============================================================================
 *
 *       Filename:  Timer.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/19/2012 03:49:10 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_NET_TIMER_H
#define MUDUO_NET_TIMER_H

#include <boost/noncopyable.hpp>

#include <muduo/base/Atomic.h>
#include <muduo/base/Timestamp.h>
#include <muduo/net/Callbacks.h>

namespace muduo 
{
namespace net 
{

class Timer : boost::noncopyable 
{
	public :
		Timer(const TimerCallback& cb, Timestamp when, double interval)
			: callback_(cb),
			  expiration_(when),
			  interval_(interval),
			  repeat_(interval_ > 0.0),
			  sequence_(s_numCreated_.incrementAndGet())
	{}

		void run() const 
		{
			callback_();
		}

		Timestamp expiration() const { return expiration_; }
		bool repeat() const { return repeat_; }
		int64_t sequence() const { return sequence_; }

		void restart(Timestamp now);

		static int64_t numCreated() { return s_numCreated_.get(); }

	private :
		const TimerCallback callback_;
		Timestamp expiration_;
		const double interval_;
		const bool repeat_;
		const int64_t sequence_;

		static AtomicInt64 s_numCreated_;
};
}
}

#endif
