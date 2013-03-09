/*
 * =============================================================================
 *
 *       Filename:  ProcessInfo_unittest.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  10/11/2012 08:25:34 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <muduo/base/ProcessInfo.h>
#include <stdio.h>
#include <vector>

using namespace muduo;
using namespace muduo::ProcessInfo;

int main()
{
	printf("pid = %d\n", ProcessInfo::pid());
	printf("uid = %d\n", ProcessInfo::uid());
	printf("pid_str = %s\n", ProcessInfo::pidString().c_str());
	printf("username = %s\n", ProcessInfo::username().c_str());
	printf("hostname = %s\n", ProcessInfo::hostname().c_str());
	printf("process status = %s\n", ProcessInfo::procStatus().c_str());
	printf("t_numOpenedFiles = %d\n", ProcessInfo::openedFiles());
	std::vector<pid_t> pids = ProcessInfo::threads();
	std::vector<pid_t>::iterator iter;
	for(iter = pids.begin(); iter != pids.end(); iter++) 
	{
		printf("pid = %d\n", *iter);
	}
}
