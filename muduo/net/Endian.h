/*
 * =============================================================================
 *
 *       Filename:  Endian.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/17/2012 04:25:21 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_NET_ENDIAN_H
#define MUDUO_NET_ENDIAN_H

#include <stdint.h>
#include <endian.h>

namespace muduo 
{
namespace net
{
namespace sockets
{
// the inline assembler code makes type blur,
// so we disable warnings for a while.
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wold-style-cast"

inline uint64_t hostToNetwork64(uint64_t host64)
{
	return htobe64(host64);
}

inline uint32_t hostToNetwork32(uint32_t host32)
{
	return htobe32(host32);
}

inline uint16_t hostToNetwork16(uint16_t host16)
{
	return htobe16(host16);
}

inline uint64_t networkToHost64(uint64_t net64)
{
	return be64toh(net64);
}

inline uint32_t networkToHost32(uint32_t net32)
{
	return be32toh(net32);
}

inline uint16_t networkToHost16(uint16_t net16)
{
	return be16toh(net16);
}

#pragma GCC diagnostic error "-Wconversion"
#pragma GCC diagnostic error "-Wold-style-cast"

}
}
}

#endif
