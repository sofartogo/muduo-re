/*
 * =============================================================================
 *
 *       Filename:  Exception_unittest.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  10/11/2012 05:41:52 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <muduo/base/Exception.h>
#include <stdio.h>

class Bar
{
	public :
		void test()
		{
			throw muduo::Exception("oops");
		}
};

void foo()
{
	Bar b;
	b.test();
}

int main()
{
	try
	{
		foo();
	}
	catch (const muduo::Exception& ex)
	{
		printf("reason: %s\n", ex.what());
		printf("stack trace: %s\n", ex.stackTrace());
	}
}
