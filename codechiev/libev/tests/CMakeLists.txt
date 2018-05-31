add_executable (tcpserver_test tcpserver_test.cpp)
target_link_libraries (tcpserver_test ccv_ev)
add_test(tcpserver_test ${CMAKE_CURRENT_BINARY_DIR}/tcpserver_test)
