/*
 * =============================================================================
 *
 *       Filename:  Condition.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/12/2012 02:52:40 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_BASE_CONDITION_H
#define MUDUO_BASE_CONDITION_H

#include <muduo/base/Mutex.h>

#include <boost/noncopyable.hpp>
#include <pthread.h>

namespace muduo 
{
class Condition : boost::noncopyable 
{
	public :
		explicit Condition(MutexLock& mutex) 
			: mutex_(mutex)
		{
			pthread_cond_init(&pcond_, NULL);
		}

		~Condition()
		{
			pthread_cond_destroy(&pcond_);
		}

		void wait()
		{
			pthread_cond_wait(&pcond_, mutex_.getPthreadMutex());
		}

		void notify()
		{
			pthread_cond_signal(&pcond_);
		}

		void notifyAll()
		{
			pthread_cond_broadcast(&pcond_);
		}

	private :
		MutexLock& mutex_;
		pthread_cond_t pcond_;
};
}

#endif
