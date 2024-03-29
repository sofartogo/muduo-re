/*
 * =============================================================================
 *
 *       Filename:  Logging.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/10/2012 03:38:29 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#ifndef MUDUO_LOGGING_H
#define MUDUO_LOGGING_H

#include <ostream>
#include <boost/scoped_ptr.hpp>

namespace muduo 
{

class LoggerImpl;
class Logger
{
	public :
		enum LogLevel
		{
			TRACE,
			DEBUG,
			INFO,
			WARN,
			ERROR,
			FATAL,
			NUM_LOG_LEVELS,
		};
	
		Logger(const char* file, int line);
		Logger(const char* file, int line, LogLevel level);
		Logger(const char* file, int line, LogLevel, const char* func);
		Logger(const char* file, int line, bool toAbort);
		~Logger();

		std::ostream& stream();
		static LogLevel logLevel();
		static void setLogLevel(LogLevel level);

	private :
		boost::scoped_ptr<LoggerImpl> impl_;
};

#define LOG_TRACE if (muduo::Logger::logLevel() <= muduo::Logger::TRACE) \
	muduo::Logger(__FILE__, __LINE__, muduo::Logger::TRACE, __func__).stream()

#define LOG_DEBUG if (muduo::Logger::logLevel() <= muduo::Logger::DEBUG) \
	muduo::Logger(__FILE__, __LINE__, muduo::Logger::DEBUG, __func__).stream()

#define LOG_INFO if (muduo::Logger::logLevel() <= muduo::Logger::INFO) \
	muduo::Logger(__FILE__, __LINE__).stream()

#define LOG_WARN muduo::Logger(__FILE__, __LINE__, muduo::Logger::WARN).stream()

#define LOG_ERROR muduo::Logger(__FILE__, __LINE__, muduo::Logger::ERROR).stream()

#define LOG_FATAL muduo::Logger(__FILE__, __LINE__, muduo::Logger::FATAL).stream()

#define LOG_SYSERR muduo::Logger(__FILE__, __LINE__, false).stream()

#define LOG_SYSFATAL muduo::Logger(__FILE__, __LINE__, true).stream()

const char* strerror_tl(int savedErrno);

// Taken from glog/logging.h
//
// Check that the input is non NULL.  This is very useful in construct or 
//
// initializer lists.

#define CHECK_NOTNULL(val) \
	::muduo::CheckNotNull(__FILE__, __LINE__, "'" #val "'must be not NULL", (val))

//A small helper for CHECK_NOTNULL().
template <typename T>
T* CheckNotNull(const char *file, int line, const char *names, T* ptr)
{
	if(ptr == NULL) {
		Logger(file, line, Logger::FATAL).stream() << names;
	}
	return ptr;
}

}

#endif
