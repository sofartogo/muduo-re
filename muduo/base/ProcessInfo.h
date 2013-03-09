/*
 * =============================================================================
 *
 *       Filename:  ProcessInfo.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/11/2012 05:53:48 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_BASE_PROCESSINFO_H
#define MUDUO_BASE_PROCESSINFO_H 

#include <muduo/base/Types.h>
#include <vector>

namespace muduo 
{

namespace ProcessInfo 
{
	pid_t pid();
	string pidString();
	uid_t uid();
	string username();
	string hostname();

	string procStatus();

	int openedFiles();
	std::vector<pid_t> threads();
}
}

#endif
