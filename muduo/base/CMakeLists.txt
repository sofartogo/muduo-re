set(base_SRCS
	CountDownLatch.cc
	Date.cc
	Exception.cc
	Logging.cc
	ProcessInfo.cc
	Thread.cc
	ThreadPool.cc
	Timestamp.cc
	)

add_library(muduo_base ${base_SRCS})
target_link_libraries(muduo_base pthread)

add_subdirectory(test)
