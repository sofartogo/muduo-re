cmake_minimum_required(VERSION 2.6)

project(muduo CXX)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TPE "Debug")
endif()

set(CXX_FLAGS
	-g
	# -DVALGRIND
	# -DMUDUO_STD_STRING
	-Wall
	-Wextra
	# -m32
	-Werror
	-Wconversion
	-Wno-unused-parameter
	-Wold-style-cast
	-Woverloaded-virtual
	-Wpointer-arith
	-Wshadow
	-Wwrite-strings
	-march=native
	# -MMD
	# -std=c++0x
	-rdynamic
	)
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(CMAKE_CXX_COMPILE "g++")
#set(CMAKE_CXX_COMPILE "ipcp")
set(CMAKE_CXX_FLAG_DEBUG "-O0")
set(CMAKE_CXX_FLAG_RELEASE "-O2 -finline-limit=1000 -DNDEBUG")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

find_package(Boost REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(muduo/base)
add_subdirectory(muduo/net)
add_subdirectory(examples)
