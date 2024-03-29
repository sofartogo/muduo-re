/*
 * =============================================================================
 *
 *       Filename:  SocketsOps.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/17/2012 05:39:19 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_NET_SOCKETSOPS_H
#define MUDUO_NET_SOCKETSOPS_H

#include <arpa/inet.h>
#include <endian.h>

namespace muduo 
{
namespace net 
{
namespace sockets 
{

///
/// Creates a non-blocking socket file descriptor,
/// abort if any error.
int createNonblockingOrDie();

int connect(int sockfd, const struct sockaddr_in& addr);
void bindOrDie(int sockfd, const struct sockaddr_in& addr);
void listenOrDie(int sockfd);
int accept(int sockfd, struct sockaddr_in* addr);
ssize_t read(int sockfd, void* buf, size_t count);
ssize_t readv(int sockfd, const struct iovec *iov, int iovcnt);
ssize_t write(int sockfd, const void* buf, size_t count);
void close(int sockfd);
void shutdownWrite(int sockfd);

void toHostPort(char *buf, size_t size,
		const struct sockaddr_in& addr);
void fromHostPort(const char* ip, uint16_t port,
		struct sockaddr_in* addr);

int getSocketError(int sockfd);

struct sockaddr_in getLocalAddr(int sockfd);
struct sockaddr_in getPeerAddr(int sockfd);
bool isSelfConnect(int sockfd);
}
}
}
#endif
