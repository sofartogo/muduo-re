/*
 * =============================================================================
 *
 *       Filename:  InetAddress.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/17/2012 04:41:03 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_NET_INETADDRESS_H
#define MUDUO_NET_INETADDRESS_H

#include <muduo/base/copyable.h>
#include <muduo/base/Types.h>

#include <netinet/in.h>

namespace muduo 
{
namespace net 
{
///
/// Wrapper of sockaddr_in.
///
/// This is an POD interface class.
class InetAddress : public muduo::copyable
{
	public :
		explicit InetAddress(uint16_t port);

		InetAddress(const string& ip, uint16_t port);

		InetAddress(const struct sockaddr_in& addr) 
			: addr_(addr)
		{}

		string toHostPort() const;

		const struct sockaddr_in& getSockAddrInet() const { return addr_; }
		void setSockAddrInet(const struct sockaddr_in& addr) { addr_ = addr; }

	private :
		struct sockaddr_in addr_;
};
}
}

#endif
