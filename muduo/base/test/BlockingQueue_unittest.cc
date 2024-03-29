/*
 * =============================================================================
 *
 *       Filename:  BlockingQueue_unittest.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  10/15/2012 03:12:55 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <muduo/base/BlockingQueue.h>
#include <muduo/base/CountDownLatch.h>
#include <muduo/base/Thread.h>

#include <boost/bind.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <string>
#include <stdio.h>

class Test
{
	public :
		Test(int numThreads)
			: latch_(numThreads),
			threads_(numThreads)
		{
			for(int i = 0; i < numThreads; i++)
			{
				char name[32];
				snprintf(name, sizeof name, "work thread %d", i);
				threads_.push_back(new muduo::Thread(
							boost::bind(&Test::threadFunc, this), muduo::string(name)));
			}
			for_each(threads_.begin(), threads_.end(), boost::bind(&muduo::Thread::start, _1));
		}

		void run(int times)
		{
			printf("waiting for count down latch\n");
			latch_.wait();
			printf("all threads started\n");
			for(int i = 0; i < times; i++)
			{
				char buf[32];
				snprintf(buf, sizeof buf, "hello %d", i);
				queue_.put(buf);
			}
		}

		void joinAll()
		{
			for(size_t i = 0; i < threads_.size(); i++)
			{
				queue_.put("stop");
			}

			for_each(threads_.begin(), threads_.end(), boost::bind(&muduo::Thread::join, _1));
		}

	private :
		void threadFunc()
		{
			printf("tid = %d, %s started\n",
					muduo::CurrentThread::tid(),
					muduo::CurrentThread::name());

			latch_.countDown();
			bool running = true;
			while(running)
			{
				std::string d(queue_.take());
				printf("tid = %d, data = %s\n", 
						muduo::CurrentThread::tid(),
						d.c_str());
				running = (d != "stop");
			}

			printf("tid = %d, %s stopped\n",
					muduo::CurrentThread::tid(),
					muduo::CurrentThread::name());
		}

		muduo::BlockingQueue<std::string> queue_;
		muduo::CountDownLatch latch_;
		boost::ptr_vector<muduo::Thread> threads_;
};


int main()
{
	printf("pid = %d, tid = %d, name = %s\n", ::getpid(), muduo::CurrentThread::tid(), muduo::CurrentThread::name());

	Test t(5);
	t.run(100);
	t.joinAll();

	printf("number of created threads %d\n", muduo::Thread::numCreated());
}
