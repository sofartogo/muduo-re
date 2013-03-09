/*
 * =============================================================================
 *
 *       Filename:  CountDownLatch.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  10/15/2012 03:17:20 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <muduo/base/CountDownLatch.h>

using namespace muduo;

CountDownLatch::CountDownLatch(int count)
	: mutex_(),
	condition_(mutex_),
	count_(count)
{}

void CountDownLatch::wait()
{
	MutexLockGuard lock(mutex_);
	while(count_ > 0) {
		condition_.wait();
	}
}

void CountDownLatch::countDown()
{
	MutexLockGuard lock(mutex_);
	--count_;
	if(count_ == 0) {
		condition_.notifyAll();
	}
}

int CountDownLatch::getCount() const 
{
	MutexLockGuard lock(mutex_);
	return count_;
}

