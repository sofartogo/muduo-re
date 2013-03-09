/*
 * =============================================================================
 *
 *       Filename:  Singleton_test.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  10/16/2012 09:18:33 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <muduo/base/Singleton.h>
#include <muduo/base/Thread.h>

#include <boost/noncopyable.hpp>
#include <stdio.h>

class Test : boost::noncopyable
{
	public :
		Test()
		{
			printf("tid = %d, constructing %p\n", muduo::CurrentThread::tid(), this);
		}

		~Test()
		{
			printf("tid = %d, destructing %p %s\n", muduo::CurrentThread::tid(), this, name_.c_str());
		}

		const std::string& name() const { return name_; }
		void setName(const std::string& n) { name_ = n; }

	private :
		std::string name_;
};

void threadFunc()
{
	printf("tid = %d, %p name = %s\n",
			muduo::CurrentThread::tid(),
			&muduo::Singleton<Test>::instance(),
			muduo::Singleton<Test>::instance().name().c_str());
	muduo::Singleton<Test>::instance().setName("only one, changed");
}

int main()
{
	muduo::Singleton<Test>::instance().setName("only one");
	muduo::Thread t1(threadFunc);
	t1.start();
	t1.join();
	printf("tid = %d, %p, name = %s\n",
			muduo::CurrentThread::tid(),
			&muduo::Singleton<Test>::instance(),
			muduo::Singleton<Test>::instance().name().c_str());
}
