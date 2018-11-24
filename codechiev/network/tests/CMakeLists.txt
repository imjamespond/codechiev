SET (UnitTest epoll_test)
add_executable (${UnitTest} ${UnitTest}.cpp)
target_link_libraries (${UnitTest} ccv_base ccv_net)
add_test(${UnitTest} ${CMAKE_CURRENT_BINARY_DIR}/${UnitTest})

# SET (UnitTest epoll_example)
# add_executable (${UnitTest} ${UnitTest}.cpp)
# target_link_libraries (${UnitTest})
# add_test(${UnitTest} ${CMAKE_CURRENT_BINARY_DIR}/${UnitTest})

# SET (UnitTest tunnel_test)
# add_executable (${UnitTest} ${UnitTest}.cpp TunnelChannel.cpp)
# target_link_libraries (${UnitTest} ccv_base ccv_net)
# add_test(${UnitTest} ${CMAKE_CURRENT_BINARY_DIR}/${UnitTest})

# SET (UnitTest speed_test)
# add_executable (${UnitTest} ${UnitTest}.cpp)
# target_link_libraries (${UnitTest} ccv_base ccv_net)
# add_test(${UnitTest} ${CMAKE_CURRENT_BINARY_DIR}/${UnitTest})

SET (UnitTest pipe_event_test)
add_executable (${UnitTest} ${UnitTest}.cpp)
target_link_libraries (${UnitTest} ccv_base ccv_net)
add_test(${UnitTest} ${CMAKE_CURRENT_BINARY_DIR}/${UnitTest})