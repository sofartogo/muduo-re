/*
 * =============================================================================
 *
 *       Filename:  Mutex.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/12/2012 02:28:39 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_BASE_MUTEX_H
#define MUDUO_BASE_MUTEX_H

#include <boost/noncopyable.hpp>
#include <pthread.h>

namespace muduo 
{

class MutexLock : boost::noncopyable
{
	public :
		MutexLock()
		{
			pthread_mutex_init(&mutex_, NULL);
		}

		~MutexLock()
		{
			pthread_mutex_destroy(&mutex_);
		}

		void lock()
		{
			pthread_mutex_lock(&mutex_);
		}

		void unlock()
		{
			pthread_mutex_unlock(&mutex_);
		}

		pthread_mutex_t* getPthreadMutex() /* non-const */
		{
			return &mutex_;
		}
	
	private :
		pthread_mutex_t mutex_;
};

class MutexLockGuard : boost::noncopyable
{
	public :
		explicit MutexLockGuard(MutexLock& mutex)
			: mutex_(mutex)
		{
			mutex_.lock();
		}

		~MutexLockGuard()
		{
			mutex_.unlock();
		}
	
	private :
		MutexLock& mutex_;
	
};
}

#endif
