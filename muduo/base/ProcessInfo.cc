/*
 * =============================================================================
 *
 *       Filename:  ProcessInfo.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  10/11/2012 06:04:34 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <muduo/base/ProcessInfo.h>

#include <algorithm>

#include <dirent.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

namespace muduo
{

namespace detail
{
	__thread int t_numOpenedFiles = 0;
	int fdDirFilter(const struct dirent* d)
	{
		if(::isdigit(d->d_name[0]))
		{
			++ t_numOpenedFiles;
		}
		return 0;
	}

	__thread std::vector<pid_t>* t_pids = NULL;

	int taskDirFilter(const struct dirent* d)
	{
		if(::isdigit(d->d_name[0]))
		{
			t_pids->push_back(atoi(d->d_name));
		}
		return 0;
	}
}

}


using namespace muduo;
using namespace muduo::detail;

pid_t ProcessInfo::pid()
{
	return ::getpid();
}

string ProcessInfo::pidString()
{
	char buf[32];
	snprintf(buf, sizeof buf, "%d", pid());
	return buf;
}

uid_t ProcessInfo::uid()
{
	return ::getuid();
}

string ProcessInfo::username()
{
	struct passwd pwd;
	struct passwd* result = NULL;
	char buf[8192];
	const char* name = "unknowuser";

	getpwuid_r(uid(), &pwd, buf, sizeof buf, &result);
	if(result)
	{
		name = pwd.pw_name;
	}
	return name;
}

string ProcessInfo::hostname()
{
	char buf[64] = "unknowhost";
	buf[sizeof(buf) - 1] = '\0';
	gethostname(buf, sizeof buf);
	return buf;
}

string ProcessInfo::procStatus()
{
	string result;
	FILE* fp = fopen("/proc/self/status", "r");
	if(fp)
	{
		while(!feof(fp))
		{
			char buf[8192];
			size_t n = fread(buf, 1, sizeof buf, fp);
			result.append(buf, n);
		}
		fclose(fp);
	}
	return result;
}

int ProcessInfo::openedFiles()
{
	t_numOpenedFiles = 0;
	struct dirent** namelist;
	scandir("/proc/self/fd", &namelist, fdDirFilter, alphasort);
	return t_numOpenedFiles;
}

std::vector<pid_t> ProcessInfo::threads()
{
	std::vector<pid_t> result;
	t_pids = &result;
	struct dirent** namelist;
	scandir("/proc/self/task", &namelist, taskDirFilter, alphasort);
	std::sort(result.begin(), result.end());
	return result;
}
