/*
 * =============================================================================
 *
 *       Filename:  Poll.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/22/2012 08:01:04 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_NET_POLLER_H
#define MUDUO_NET_POLLER_H

#include <vector>
#include <boost/noncopyable.hpp>

#include <muduo/base/Timestamp.h>
#include <muduo/net/EventLoop.h>

namespace muduo 
{
namespace net 
{
class Channel;
class Poller : boost::noncopyable 
{
	public :
		typedef std::vector<Channel*> ChannelList;

		Poller(EventLoop* loop);
		virtual ~Poller();

		virtual Timestamp poll(int timeoutMs, ChannelList* activeChannels) = 0;
		virtual void updateChannel(Channel* channel) = 0;
		virtual void removeChannel(Channel* Channel) = 0;

		static Poller* newDefaultPoller(EventLoop* loop);

		void assertInLoopThread()
		{
			ownerLoop_->assertInLoopThread();
		}

	private :
		EventLoop* ownerLoop_;
};

}
}

#endif
