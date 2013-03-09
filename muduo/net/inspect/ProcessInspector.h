/*
 * =============================================================================
 *
 *       Filename:  ProcessInspector.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/14/2012 04:28:07 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_NET_INSPECT_PROCESSINSPECTOR_H
#define MUDUO_NET_INSPECT_PROCESSINSPECTOR_H

#include <muduo/net/inspect/Inspector.h>
#include <boost/noncopyable.hpp>

namespace muduo 
{
namespace net 
{
class ProcessInspector : boost::noncopyable 
{
	public :
		void registerCommands(Inspector* ins);

	private :
		static string pid(HttpRequest::Method, const Inspector::ArgList&);
		static string procStatus(HttpRequest::Method, const Inspector::ArgList&);
		static string openedFiles(HttpRequest::Method, const Inspector::ArgList&);
		static string threads(HttpRequest::Method, const Inspector::ArgList&);

};
}
}

#endif
