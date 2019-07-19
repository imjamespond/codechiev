SET (UnitTest eventloop_test)
add_executable (${UnitTest} ${UnitTest}.cpp)
target_link_libraries (${UnitTest} cc_base cc_net)

SET (UnitTest epoll_test)
add_executable (${UnitTest} ${UnitTest}.cpp)
target_link_libraries (${UnitTest} cc_base cc_net)
 
SET (UnitTest timer_test)
add_executable (${UnitTest} ${UnitTest}.cpp)
target_link_libraries (${UnitTest} cc_base cc_net)