/*
 * =============================================================================
 *
 *       Filename:  Socket.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/17/2012 05:02:08 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_NET_SOCKET_H
#define MUDUO_NET_SOCKET_H

#include <boost/noncopyable.hpp>

namespace muduo 
{
namespace net 
{

class InetAddress;

///
/// Wrapper of socket file descriptor.
///
/// It closes the sockfd when desctructs.
/// It's thread safe, all operations are delagated to OS.
class Socket : boost::noncopyable 
{
	public :
		explicit Socket(int sockfd)
			: sockfd_(sockfd)
		{}

		~Socket();

		int fd() const { return sockfd_; }

		void bindAddress(const InetAddress& localaddr);

		void listen();

		int accept(InetAddress* peeraddr);

		void shutdownWrite();

		void setTcpNoDelay(bool on);

		void setReuseAddr(bool on);

	private :
		const int sockfd_;
};

}
}

#endif
