/*
 * =============================================================================
 *
 *       Filename:  timer.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  11/19/2012 02:20:46 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <muduo/net/EventLoop.h>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>

class Printer : boost::noncopyable
{
	public :
		Printer(muduo::net::EventLoop* loop)
			: loop_(loop),
			  count_(0)
		{
			loop_->runAfter(1, boost::bind(&Printer::print, this));
		}

		~Printer()
		{
			std::cout << "Final count is " << count_ << "\n";
		}

		void print()
		{
			if(count_ < 5)
			{
				std::cout << count_ << "\n";
				++count_;

				loop_->runAfter(1, boost::bind(&Printer::print, this));
			}
			else 
			{
				loop_->quit();
			}
		}

	private :
		muduo::net::EventLoop* loop_;
		int count_;
};


int main()
{
	muduo::net::EventLoop loop;
	Printer printer(&loop);
	loop.loop();
}
