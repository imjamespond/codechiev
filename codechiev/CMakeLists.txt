cmake_minimum_required(VERSION 2.6)

PROJECT (CodeChiev)

message("message root directory:${CMAKE_SOURCE_DIR}")
message("message binary directory:${CMAKE_BINARY_DIR}")

add_definitions(-DUseEpoll=1)
add_definitions(-DUseEpollET=1)
add_definitions(-DLoggerDebug=1)
add_definitions(-DLoggerThread=1)
add_definitions(-DLoggerFile=1)
add_definitions(-DLoggerFunc=1)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")#03would be faster
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -finline-limit=1000 -DNDEBUG")
#set(CMAKE_BUILD_TYPE Release)

set (BOOST ${BOOST_ROOT})
set (HEADER ${CMAKE_CURRENT_SOURCE_DIR})
set (HEADER ${HEADER} ${BOOST})
set (HEADER ${HEADER} ${ROOT})
include_directories (${HEADER})

set (MY_LIBS pthread rt)

add_subdirectory(base)
add_subdirectory(net)