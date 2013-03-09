/*
 * =============================================================================
 *
 *       Filename:  client.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  11/21/2012 03:46:49 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <muduo/net/TcpClient.h>

#include <muduo/base/Logging.h>
#include <muduo/base/Thread.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>

#include <boost/bind.hpp>

#include <utility>

#include <mcheck.h>
#include <stdio.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

class EchoClient : boost::noncopyable
{
 public:
  EchoClient(EventLoop* loop, const InetAddress& listenAddr, int size)
    : loop_(loop),
      client_(loop, listenAddr, "EchoClient"),
      message_(size, 'H')
  {
    client_.setConnectionCallback(
        boost::bind(&EchoClient::onConnection, this, _1));
    client_.setMessageCallback(
        boost::bind(&EchoClient::onMessage, this, _1, _2, _3));
    //client_.enableRetry();
  }

  void connect()
  {
    client_.connect();
  }

 private:
  void onConnection(const TcpConnectionPtr& conn)
  {
    LOG_TRACE << conn->localAddress().toHostPort() << " -> "
        << conn->peerAddress().toHostPort() << " is "
        << (conn->connected() ? "UP" : "DOWN");

    conn->send(message_);
  }

  void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time)
  {
    conn->send(buf);
  }

  EventLoop* loop_;
  TcpClient client_;
  string message_;
};

int main(int argc, char* argv[])
{
  LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();
  if (argc > 1)
  {
    EventLoop loop;
    InetAddress serverAddr(argv[1], 2007);

    int size = 256;
    if (argc > 2)
    {
      size = atoi(argv[2]);
    }

    EchoClient client(&loop, serverAddr, size);
    client.connect();
    loop.loop();
  }
  else
  {
    printf("Usage: %s host_ip [msg_size]\n", argv[0]);
  }
}


