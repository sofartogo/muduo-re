/*
 * =============================================================================
 *
 *       Filename:  Thread.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/15/2012 03:25:19 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_BASE_THREAD_H
#define MUDUO_BASE_THREAD_H

#include <muduo/base/Atomic.h>
#include <muduo/base/Types.h>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <pthread.h>

namespace muduo 
{

class Thread : boost::noncopyable
{
	public :
		typedef boost::function<void ()> ThreadFunc;

		explicit Thread(const ThreadFunc&, const string& name = string());

		~Thread();

		void start();
		void join();

		bool started() const { return started_; }
		pid_t tid() const { return tid_; }
		const string& name() const { return name_; }

		static int numCreated() { return numCreated_.get(); }
		
	private :
		static void* startThread(void* thread);
		void runInThread();
		
		bool started_;
		pthread_t pthreadId_;
		pid_t tid_;
		ThreadFunc func_;
		string name_;

		static AtomicInt32 numCreated_;
};

namespace CurrentThread
{
	pid_t tid();
	const char* name();
	bool isMainThread();
}
}

#endif
