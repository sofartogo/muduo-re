/*
 * =============================================================================
 *
 *       Filename:  PollPoller.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/23/2012 10:40:08 AM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_NET_POLLER_POLLPOLLER_H
#define MUDUO_NET_POLLER_POLLPOLLER_H

#include <muduo/net/Poller.h>

#include <map>
#include <vector>

struct pollfd;
namespace muduo 
{
namespace net 
{
class PollPoller : public Poller 
{
	public :
		PollPoller(EventLoop* loop);
		virtual ~PollPoller();

		virtual Timestamp poll(int timeoutMs, ChannelList* activeChannels);
		virtual void updateChannel(Channel* channel);
		virtual void removeChannel(Channel* channel);

	private :
		void fillActiveChannels(int numEvents,
				ChannelList* activeChannels) const;
		typedef std::vector<struct pollfd> PollFdList;
		typedef std::map<int, Channel*> ChannelMap;
		PollFdList pollfds_;
		ChannelMap channels_;
};
}
}

#endif
