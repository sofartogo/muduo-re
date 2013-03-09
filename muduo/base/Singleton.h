/*
 * =============================================================================
 *
 *       Filename:  Singleton.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/16/2012 09:21:39 AM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_BASE_SINGLETON_H
#define MUDUO_BASE_SINGLETON_H

#include <boost/noncopyable.hpp>
#include <pthread.h>
#include <stdlib.h>

namespace muduo 
{

template<typename T>
class Singleton : boost::noncopyable 
{
	public :
		static T& instance()
		{
			pthread_once(&ponce_, &Singleton::init);
			return *value_;
		}

	private :
		Singleton();
		~Singleton();

		static void init()
		{
			value_ = new T();
			::atexit(destroy);
		}

		static void destroy()
		{
			delete value_;
		}

	private :
		static pthread_once_t ponce_;
		static T*             value_;
};

template<typename T>
pthread_once_t Singleton<T>::ponce_ = PTHREAD_ONCE_INIT;

template<typename T>
T* Singleton<T>::value_ = NULL;
}

#endif
