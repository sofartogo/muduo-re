/*
 * =============================================================================
 *
 *       Filename:  BlockingQueue.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/15/2012 02:19:07 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_BASE_BLOCKINGQUEUE_H
#define MUDUO_BASE_BLOCKINGQUEUE_H

#include <muduo/base/Condition.h>
#include <muduo/base/Mutex.h>

#include <boost/noncopyable.hpp>
#include <deque>
#include <assert.h>

namespace muduo 
{

template<typename T>
class BlockingQueue : boost::noncopyable
{

	public :
		BlockingQueue()
			: mutex_(),
			  cond_(mutex_),
			  queue_()
		{}

		void put(const T& x)
		{
			MutexLockGuard lock(mutex_);
			queue_.push_back(x);
			cond_.notify();
		}

		T take()
		{
			MutexLockGuard lock(mutex_);
			while(queue_.empty())
			{
				cond_.wait();
			}
			assert(!queue_.empty());
			T front(queue_.front());
			queue_.pop_front();
			return front;
		}

		size_t size() const 
		{
			MutexLockGuard lock(mutex_);
			return queue_.size();
		}

	private :
		mutable MutexLock mutex_;
		Condition         cond_;
		std::deque<T>     queue_;
};
}

#endif
