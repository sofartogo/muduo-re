/*
 * =============================================================================
 *
 *       Filename:  download.cc
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  11/19/2012 03:09:25 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>

#include <stdio.h>

using namespace muduo;
using namespace muduo::net;

const char* g_file = NULL;

string readFile(const char* filename)
{
	string content;
	FILE* fp = ::fopen(filename, "rb");
	if(fp)
	{
		const int kBufSize = 1024*1024;
		char iobuf[kBufSize];
		::setbuffer(fp, iobuf, sizeof iobuf);

		char buf[kBufSize];
		size_t nread = 0;
		while((nread = ::fread(buf, 1, sizeof buf, fp)) > 0)
		{
			content.append(buf, nread);
		}
		::fclose(fp);
	}
	return content;
}

void onConnection(const TcpConnectionPtr& conn)
{
	LOG_INFO << "FileServer - " << conn->peerAddress().toHostPort() << " -> "
		<< conn->localAddress().toHostPort() << " is "
		<< (conn->connected() ? "UP" : "DOWN");
	if(conn->connected())
	{
		LOG_INFO << "FileServer - Sending file " << g_file << " to " << conn->peerAddress().toHostPort();
		string fileContent = readFile(g_file);
		conn->send(fileContent);
		conn->shutdown();
		LOG_INFO << "FileServer - done";
	}
}

int main(int argc, char* argv[])
{
	LOG_INFO << "pid = " << getpid();

	if(argc > 1)
	{
		g_file = argv[1];
		EventLoop loop;
		InetAddress listenAddr(2021);
		TcpServer server(&loop, listenAddr, "FileServer");
		server.setConnectionCallback(onConnection);
		server.start();
		loop.loop();
	}
	else 
	{
		fprintf(stderr, "Usage : %s file_for_downloading\n", argv[0]);
	}
}
