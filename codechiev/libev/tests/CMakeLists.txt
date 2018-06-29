# add_executable (tcpserver_test tcpserver_test.cpp)
# target_link_libraries (tcpserver_test ccv_ev)
# add_test(tcpserver_test ${CMAKE_CURRENT_BINARY_DIR}/tcpserver_test)

# add_executable (tcpclient_test tcpclient_test.cpp)
# target_link_libraries (tcpclient_test ccv_ev)
# add_test(tcpclient_test ${CMAKE_CURRENT_BINARY_DIR}/tcpclient_test)

# add_executable (timer_test timer_test.cpp)
# target_link_libraries (timer_test ccv_ev)
# add_test(tcpclient_test ${CMAKE_CURRENT_BINARY_DIR}/timer_test)

# add_executable (signal_test signal_test.cpp)
# target_link_libraries (signal_test ccv_ev)
# add_test(tcpclient_test ${CMAKE_CURRENT_BINARY_DIR}/signal_test)

SET (UnitTest chatroom_server)
add_executable (${UnitTest} ${UnitTest}.cpp)
target_link_libraries (${UnitTest} ccv_ev)
add_test(${UnitTest} ${CMAKE_CURRENT_BINARY_DIR}/${UnitTest})