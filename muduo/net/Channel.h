/*
 * =============================================================================
 *
 *       Filename:  Channel.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/22/2012 09:48:36 AM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef MUDUO_NET_CHANNEL_H
#define MUDUO_NET_CHANNEL_H

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <muduo/base/Timestamp.h>

namespace muduo 
{
namespace net 
{
class EventLoop;

///
/// A selectable I/O channel.
///
/// This class doesn't own the file descriptor.
/// The file descriptor could be a socket,
/// an eventfd, a timerfd, or a signalfd

class Channel : boost::noncopyable 
{
	public :
		typedef boost::function<void ()> EventCallback;
		typedef boost::function<void (Timestamp)> ReadEventCallback;

		Channel(EventLoop* loop, int fd);
		~Channel();

		void handleEvent(Timestamp receiveTime);
		void setReadCallback(const ReadEventCallback& cb)
		{ readCallback_ = cb; }
		void setWriteCallback(const EventCallback& cb)
		{ writeCallback_ = cb; }
		void setCloseCallback(const EventCallback& cb)
		{ closeCallback_ = cb; }
		void setErrorCallback(const EventCallback& cb)
		{ errorCallback_ = cb; }

		/// Tie this channel to the owner object managed by shared_ptr,
  		/// prevent the owner object being destroyed in handleEvent.
		void tie(const boost::shared_ptr<void>&);

		int fd() const { return fd_; }
		int events() const { return events_; }
		void set_revents(int revt) { revents_ = revt; }

		bool isNoneEvent() const { return events_ == kNoneEvent; }

		void enableReading() { events_ |= kReadEvent; update(); }

		void enableWriting() { events_ |= kWriteEvent; update(); }

		void disableWriting() { events_ |= ~kWriteEvent; update(); }
		void disableAll() { events_ |= kNoneEvent; update(); }
		
		bool isWriting() const { return events_ & kWriteEvent; }

		int index() { return index_; }
		void set_index(int idx) { index_ = idx; }

		string reventsToString() const;

		EventLoop* ownerLoop() { return loop_; }

	private :
		void update();
		void handlerEventWithGuard(Timestamp receiveTime);
		static const int kNoneEvent;
		static const int kReadEvent;
		static const int kWriteEvent;

		EventLoop* loop_;
		const int fd_;
		int events_;
		int revents_;
		int index_;

		boost::weak_ptr<void> tie_;
		bool tied_;
		bool eventHandling_;
		ReadEventCallback readCallback_;
		EventCallback writeCallback_;
		EventCallback closeCallback_;
		EventCallback errorCallback_;

};
}
}
#endif
