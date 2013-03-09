/*
 * =============================================================================
 *
 *       Filename:  Exception.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/11/2012 05:10:37 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_BASE_EXCEPTION_H
#define MUDUO_BASE_EXCEPTION_H

#include <muduo/base/Types.h>
#include <exception>

namespace muduo 
{

class Exception : public std::exception
{
	public :
		explicit Exception(const char * what);
		explicit Exception(const string& what);

		virtual ~Exception() throw();
		virtual const char* what() const throw();
		const char* stackTrace() const throw();

	private:
		void fillStackTrace();

		string message_;
		string stack_;
};
}

#endif
