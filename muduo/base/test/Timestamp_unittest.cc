/*
 * =============================================================================
 *
 *       Filename:  Timestamp_unittest.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  10/11/2012 11:58:29 AM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <muduo/base/Timestamp.h>
#include <stdio.h>
using namespace std;
using muduo::Timestamp;

void passByValue(Timestamp x)
{
	printf("%s\n", x.toString().c_str());
}

void passByConstReference(const Timestamp& x)
{
	printf("%s\n", x.toString().c_str());
}

int main()
{
	Timestamp now(Timestamp::now());
	printf("%s\n", now.toString().c_str());
	passByValue(now);
	passByConstReference(now);
	printf("%s\n", now.toFormattedString().c_str());
}
