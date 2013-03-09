/*
 * =============================================================================
 *
 *       Filename:  ThreadPool.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/17/2012 10:46:15 AM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_BASE_THREADPOOL_H
#define MUDUO_BASE_THREADPOOL_H

#include <muduo/base/Condition.h>
#include <muduo/base/Mutex.h>
#include <muduo/base/Thread.h>
#include <muduo/base/Types.h>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include <deque>

namespace muduo 
{
class ThreadPool : boost::noncopyable 
{
	public :
		typedef boost::function<void ()> Task;

		explicit ThreadPool(const string& name = string());
		~ThreadPool();

		void start(int numThreads);
		void stop();

		void run(const Task& f);

	private :
		void runInThread();
		Task take();

		MutexLock mutex_;
		Condition cond_;
		string name_;
		boost::ptr_vector<muduo::Thread> threads_;
		std::deque<Task> queue_;
		bool running_;
};
}

#endif
