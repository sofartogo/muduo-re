set(net_SRCS
	Acceptor.cc
	boilerplate.cc
	Buffer.cc
	Channel.cc
	Connector.cc
	EventLoop.cc
	EventLoopThread.cc
	EventLoopThreadPool.cc
	InetAddress.cc
	Poller.cc
	poller/DefaultPoller.cc 
	poller/EPollPoller.cc 
	poller/PollPoller.cc
	Socket.cc
	SocketsOps.cc
	TcpClient.cc
	TcpConnection.cc
	TcpServer.cc
	Timer.cc
	TimerQueue.cc
	)
	
add_library(muduo_net ${net_SRCS})
target_link_libraries(muduo_net muduo_base)

add_subdirectory(tests)
add_subdirectory(http)
add_subdirectory(inspect)
