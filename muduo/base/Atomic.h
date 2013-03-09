/*
 * =============================================================================
 *
 *       Filename:  Atomic.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/12/2012 02:41:00 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_BASE_ATOMIC_H
#define MUDUO_BASE_ATOMIC_H

#include <boost/noncopyable.hpp>
#include <stdint.h>

namespace muduo 
{

namespace detail
{
template<typename T>
class AtomicIntegerT : boost::noncopyable 
{
	public :
		AtomicIntegerT()
			: value_(0)
		{}

		// uncomment if you need copying and assignment
	 	//
	 	// AtomicIntegerT(const AtomicIntegerT& that)
	 	//   : value_(that.get())
	 	// {}
	 	//
	 	// AtomicIntegerT& operator=(const AtomicIntegerT& that)
	 	// {
	 	//   getAndSet(that.get());
	 	//   return *this;
	 	// }
		
		T get()
		{
			return __sync_val_compare_and_swap(&value_, 0, 0);
		}

		T getAndAdd(T x)
		{
			return __sync_fetch_and_add(&value_, x);
		}

		T addAndGet(T x)
		{
			return getAndAdd(x) + x;
		}

		T incrementAndGet()
		{
			return addAndGet(1);
		}

		T decrementAndGet()
		{
			return addAndGet(-1);
		}

		void add(T x)
		{
			getAndAdd(x);
		}

		void increment()
		{
			incrementAndGet();
		}

		T getAndSet(T newValue)
		{
			return __sync_lock_test_and_set(&value_, newValue);
		}

	private :
		volatile T value_;

};
}

typedef detail::AtomicIntegerT<int32_t> AtomicInt32;
typedef detail::AtomicIntegerT<int64_t> AtomicInt64;
}

#endif
