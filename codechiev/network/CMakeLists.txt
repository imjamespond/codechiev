
FILE(GLOB SOURCE_FILES "*.cpp")
ADD_LIBRARY (cc_net ${SOURCE_FILES})
TARGET_LINK_LIBRARIES (cc_net cc_base)

SET (TcpForward tcp-forward)
add_executable (${TcpForward} ${TcpForward}/${TcpForward}.cpp ${TcpForward}/server.impl.cpp  ${TcpForward}/client.impl.cpp)
target_link_libraries (${TcpForward} cc_base cc_net)

ADD_SUBDIRECTORY(tests)
