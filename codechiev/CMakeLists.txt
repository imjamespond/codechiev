cmake_minimum_required(VERSION 2.6)

PROJECT (CodeChiev)

message("message root directory:${CMAKE_SOURCE_DIR}")
message("message binary directory:${CMAKE_BINARY_DIR}")

add_definitions(-DUSE_EPOLL=1)
add_definitions(-DLOGGER_DEBUG=1)

set (BOOST ${BOOST_ROOT})

set (HEADER ${CMAKE_CURRENT_SOURCE_DIR})
set (HEADER ${HEADER} ${BOOST})
set (HEADER ${HEADER} ${ROOT})
include_directories (${HEADER})

set (MY_LIBS pthread rt)

add_subdirectory(base)
add_subdirectory(utility)
add_subdirectory(net)
add_subdirectory(db)