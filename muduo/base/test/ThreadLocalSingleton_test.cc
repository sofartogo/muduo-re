/*
 * =============================================================================
 *
 *       Filename:  ThreadLocalSingleton_test.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  10/17/2012 10:27:32 AM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <muduo/base/ThreadLocalSingleton.h>
#include <muduo/base/Thread.h>

#include <boost/bind.hpp>
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

void threadFunc(const char* changeTo)
{
	printf("tid = %d %p name = %s\n",
			muduo::CurrentThread::tid(),
			&muduo::ThreadLocalSingleton<Test>::instance(),
			muduo::ThreadLocalSingleton<Test>::instance().name().c_str());
	muduo::ThreadLocalSingleton<Test>::instance().setName(changeTo);

	printf("tid = %d %p name = %s\n",
			muduo::CurrentThread::tid(),
			&muduo::ThreadLocalSingleton<Test>::instance(),
			muduo::ThreadLocalSingleton<Test>::instance().name().c_str());
	muduo::ThreadLocalSingleton<Test>::destroy();
}

int main()
{
	muduo::ThreadLocalSingleton<Test>::instance().setName("main one");
	muduo::Thread t1(boost::bind(threadFunc, "thread1"));
	muduo::Thread t2(boost::bind(threadFunc, "thread2"));
	t1.start();
	t2.start();
	t1.join();
	printf("tid = %d %p name = %s\n",
			muduo::CurrentThread::tid(),
			&muduo::ThreadLocalSingleton<Test>::instance(),
			muduo::ThreadLocalSingleton<Test>::instance().name().c_str());
	t2.join();

}
