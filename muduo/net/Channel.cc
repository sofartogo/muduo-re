/*
 * =============================================================================
 *
 *       Filename:  Channel.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  10/22/2012 05:58:35 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <muduo/base/Logging.h>
#include <muduo/net/Channel.h>
#include <muduo/net/EventLoop.h>

#include <sstream>
#include <poll.h>

using namespace muduo;
using namespace muduo::net;

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop* loop, int fd__)
	: loop_(loop),
	  fd_(fd__),
	  events_(0),
	  revents_(0),
	  index_(-1),
	  eventHandling_(false)
{}

Channel::~Channel()
{
	assert(!eventHandling_);
}

void Channel::tie(const boost::shared_ptr<void>& obj)
{
	tie_ = obj;
	tied_ = true;
}

void Channel::update()
{
	loop_->updateChannel(this);
}

void Channel::handleEvent(Timestamp receiveTime)
{
	boost::shared_ptr<void> guard;
	if (tied_) 
	{
		guard = tie_.lock();
		if(guard)
		{
			handlerEventWithGuard(receiveTime);
		}
	}
	else 
	{
		handlerEventWithGuard(receiveTime);
	}
}

void Channel::handlerEventWithGuard(Timestamp receiveTime)
{
	eventHandling_ = true;
	if((revents_ & POLLHUP) && !(revents_ & POLLIN))
	{
		LOG_WARN << "Channel::handle_event() POLLHUP";
		if(closeCallback_) closeCallback_();
	}

	if(revents_ & POLLNVAL)
	{
		LOG_WARN << "Channel::handle_event() POLLNVAL";
	}

	if(revents_ & (POLLERR | POLLNVAL))
	{
		if(errorCallback_) errorCallback_();
	}

	if(revents_ & (POLLIN | POLLPRI | POLLRDHUP))
	{
		if(readCallback_) readCallback_(receiveTime);
	}

	if(revents_ & POLLOUT) 
	{
		if(writeCallback_) writeCallback_();
	}

	eventHandling_ = false;
}

string Channel::reventsToString() const 
{
	std::ostringstream oss;
	oss << fd_ << ": ";
	if(revents_ & POLLIN)
		oss << "IN ";
	if(revents_ & POLLPRI)
		oss << "PRI ";
	if(revents_ & POLLOUT)
		oss << "OUT ";
	if(revents_ & POLLHUP)
		oss << "HUP ";
	if(revents_ & POLLRDHUP)
		oss << "RDHUP ";
	if(revents_ & POLLERR)
		oss << "ERR ";
	if(revents_ & POLLNVAL)
		oss << "NVAL ";

	return oss.str().c_str();
}
