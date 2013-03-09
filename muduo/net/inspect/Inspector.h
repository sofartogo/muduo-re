/*
 * =============================================================================
 *
 *       Filename:  Inspector.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/14/2012 03:30:25 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_INSPECT_INSPECTOR_H
#define MUDUO_INSPECT_INSPECTOR_H

#include <muduo/base/Mutex.h>
#include <muduo/net/http/HttpRequest.h>
#include <muduo/net/http/HttpServer.h>

#include <map>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

namespace muduo 
{
namespace net
{
class ProcessInspector;

class Inspector : boost::noncopyable 
{
	public :
		typedef std::vector<string> ArgList;
		typedef boost::function<string (HttpRequest::Method, const ArgList& atgs)> Callback;
		Inspector(EventLoop* loop,
				const InetAddress& httpAddr,
				const string& name);
		~Inspector();

		void add(const string& module,
				const string& command,
				const Callback& cb,
				const string& help);

	private :
		typedef std::map<string, Callback> CommandList;
		typedef std::map<string, string> HelpList;

		void start();
		void onRequest(const HttpRequest& req, HttpResponse* resp);

		HttpServer server_;
		boost::scoped_ptr<ProcessInspector> processInspector_;
		MutexLock mutex_;
		std::map<string, CommandList> commands_;
		std::map<string, HelpList> helps_;
};
}
}

#endif
