/*
 * =============================================================================
 *
 *       Filename:  ThreadLocalSingleton.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/17/2012 10:23:40 AM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_BASE_THREADLOCALSINGLETON_H
#define MUDUO_BASE_THREADLOCALSINGLETON_H

#include <boost/noncopyable.hpp>

namespace muduo 
{
template<typename T>
class ThreadLocalSingleton : boost::noncopyable
{
	public :
		static T& instance()
		{
			if(!t_value_)
			{
				t_value_ = new T();
			}
			return *t_value_;
		}

		static void destroy()
		{
			delete t_value_;
			t_value_ = 0;
		}

	private :
		static __thread T* t_value_;
};

template<typename T>
__thread T* ThreadLocalSingleton<T>::t_value_ = 0;
}

#endif
