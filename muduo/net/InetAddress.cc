/*
 * =============================================================================
 *
 *       Filename:  InetAddress.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  10/17/2012 05:23:13 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <muduo/net/InetAddress.h>

#include <muduo/net/Endian.h>
#include <muduo/net/SocketsOps.h>

#include <strings.h>
//#include <netinet/in.h>

#include <boost/static_assert.hpp>

// INADDR_ANY use (type)value casting.
#pragma GCC diagnostic ignored "-Wold-style-cast"
static const in_addr_t kInaddrAny = INADDR_ANY;
#pragma GCC diagnostic error "-Wold-style-cast"

//     /* Structure describing an Internet socket address.  */
//     struct sockaddr_in {
//         sa_family_t    sin_family; /* address family: AF_INET */
//         uint16_t       sin_port;   /* port in network byte order */
//         struct in_addr sin_addr;   /* internet address */
//     };

//     /* Internet address. */
//     typedef uint32_t in_addr_t;
//     struct in_addr {
//         in_addr_t       s_addr;     /* address in network byte order */
//     };

using namespace muduo;
using namespace muduo::net;

BOOST_STATIC_ASSERT(sizeof(InetAddress) == sizeof(struct sockaddr_in));

InetAddress::InetAddress(uint16_t port)
{
	bzero(&addr_, sizeof addr_);
	addr_.sin_family = AF_INET;
	addr_.sin_addr.s_addr = sockets::hostToNetwork32(kInaddrAny);
	addr_.sin_port = sockets::hostToNetwork16(port);
}

InetAddress::InetAddress(const string& ip, uint16_t port)
{
	bzero(&addr_, sizeof addr_);
	sockets::fromHostPort(ip.c_str(), port, &addr_);
}

string InetAddress::toHostPort() const 
{
	char buf[32];
	sockets::toHostPort(buf, sizeof buf, addr_);
	return buf;
}
