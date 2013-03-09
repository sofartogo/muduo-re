/*
 * =============================================================================
 *
 *       Filename:  EPollPoller.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/23/2012 02:04:37 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_NET_POLLER_EPOLLPOLLER_H
#define MUDUO_NET_POLLER_EPOLLPOLLER_H

#include <muduo/net/Poller.h>

#include <map>
#include <vector>

#include <sys/epoll.h>

namespace muduo 
{
namespace net 
{

class EPollPoller : public Poller 
{
	public :
		EPollPoller(EventLoop* loop);
		virtual ~EPollPoller();

		virtual Timestamp poll(int timeoutMs, ChannelList* activeChannels);
		virtual void updateChannel(Channel* channel);
		virtual void removeChannel(Channel* channel);

	private :
		static const int kInitEventListSize = 16;
		void fillActiveChannels(int numEvents,
				ChannelList* activeChannels) const;
		void update(int operation, Channel* channel);

		typedef std::vector<struct epoll_event> EventList;
		typedef std::map<int, Channel*> ChannelMap;

		int epollfd_;
		EventList events_;
		ChannelMap channels_;
};
}
}

#endif
