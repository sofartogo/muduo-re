/*
 * =============================================================================
 *
 *       Filename:  Exception.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  10/11/2012 05:20:17 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#include <muduo/base/Exception.h>

#include <execinfo.h>
#include <stdlib.h>

using namespace muduo;

Exception::Exception(const char* msg)
	: message_(msg)
{
	fillStackTrace();
}

Exception::Exception(const string& msg)
	: message_(msg)
{
	fillStackTrace();
}

Exception::~Exception() throw()
{}

const char* Exception::what() const throw()
{
	return message_.c_str();
}

const char* Exception::stackTrace() const throw()
{
	return stack_.c_str();
}

void Exception::fillStackTrace()
{
	const int len = 200;
	void* buffer[len];
	int nptrs = ::backtrace(buffer, len);
	char **strings = ::backtrace_symbols(buffer, nptrs);
	if(strings) 
	{
		for(int i = 0; i < nptrs; i++)
		{
			stack_.append(strings[i]);
			stack_.push_back('\n');
		}
		free(strings);
	}
}
