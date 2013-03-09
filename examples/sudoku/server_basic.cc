/*
 * =============================================================================
 *
 *       Filename:  server_basic.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  11/22/2012 02:31:50 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include "sudoku.h"
#include <muduo/base/Atomic.h>
#include <muduo/base/Logging.h>
#include <muduo/base/Thread.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpServer.h>

#include <boost/bind.hpp>
#include <utility>

#include <mcheck.h>
#include <stdio.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

class SudokuServer 
{
	public :
		SudokuServer(EventLoop* loop, const InetAddress& listenAddr)
			: loop_(loop),
			  server_(loop, listenAddr, "SudokuServer"),
			  startTime_(Timestamp::now())
		{
			server_.setConnectionCallback(
					boost::bind(&SudokuServer::onConnection, this, _1));
			server_.setMessageCallback(
					boost::bind(&SudokuServer::onMessage, this, _1, _2, _3));
		}

		void start()
		{
			server_.start();
		}

	private :
		void onConnection(const TcpConnectionPtr& conn)
		{
			LOG_TRACE << conn->peerAddress().toHostPort() << " -> "
				<< conn->localAddress().toHostPort() << " is "
				<< (conn->connected() ? "UP" : "DOWN") ;
		}

		void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time)
		{
			LOG_DEBUG << conn->name();
			size_t len = buf->readableBytes();
			while(len >= kCells + 2)
			{
				const char* crlf = buf->findCRLF();
				if(crlf)
				{
					string request(buf->peek(), crlf);
					string id;
					buf->retrieveUntil(crlf+2);
					string::iterator colon = find(request.begin(), request.end(), ':');
					if(colon != request.end())
					{
						id.assign(request.begin(), colon);
						request.erase(request.begin(), colon+1);
					}
					if(request.size() == implicit_cast<size_t>(kCells))
					{
						string result = solveSudoku(request);
						if(id.empty())
						{
							conn->send(result+"\r\n");
						}
						else 
						{
							conn->send(id+":"+result+"\r\n");
						}
					}
					else 
					{
						conn->send("Bad Request!\r\n");
						conn->shutdown();
					}
				}
				else 
				{
					break;
				}
			}
		}

		EventLoop* loop_;
		TcpServer server_;
		Timestamp startTime_;
};


int main()
{
	LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();

	EventLoop loop;
	InetAddress listenAddr(9981);
	SudokuServer server(&loop, listenAddr);

	server.start();

	loop.loop();
}
