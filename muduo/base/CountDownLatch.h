/*
 * =============================================================================
 *
 *       Filename:  CountDownLatch.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/12/2012 02:26:41 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_BASE_COUNTDOWNLATCH_H
#define MUDUO_BASE_COUNTDOWNLATCH_H

#include <muduo/base/Condition.h>
#include <muduo/base/Mutex.h>

#include <boost/noncopyable.hpp>

namespace muduo 
{

class CountDownLatch : boost::noncopyable 
{
	public :
		explicit CountDownLatch(int count);

		void wait();

		void countDown();

		int getCount() const;

	private :
		mutable MutexLock mutex_;
		Condition condition_;
		int count_;
};
}

#endif
