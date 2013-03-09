/*
 * =============================================================================
 *
 *       Filename:  Atomic_unittest.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  10/12/2012 02:57:55 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <muduo/base/Atomic.h>
#include <assert.h>

int main()
{
	{
		muduo::AtomicInt64 a0;
		assert(a0.get() == 0);
		assert(a0.getAndAdd(1) == 0);
		assert(a0.get() == 1);
		assert(a0.addAndGet(2) == 3);
		assert(a0.get() == 3);
		assert(a0.incrementAndGet() == 4);
		assert(a0.get() == 4);
		a0.increment();
		assert(a0.addAndGet(-3) == 2);
		assert(a0.getAndSet(100) == 2);
		assert(a0.get() == 100);
	}

	{
		muduo::AtomicInt32 a1;
		assert(a1.get() == 0);
		assert(a1.getAndAdd(1) == 0);
		assert(a1.get() == 1);
		assert(a1.addAndGet(2) == 3);
		assert(a1.get() == 3);
		assert(a1.incrementAndGet() == 4);
		assert(a1.get() == 4);
		a1.increment();
		assert(a1.addAndGet(-3) == 2);
		assert(a1.getAndSet(100) == 2);
		assert(a1.get() == 100);
	}
}
