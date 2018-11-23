
add_executable (fixedbuffer_test fixedbuffer_test.cpp)
target_link_libraries (fixedbuffer_test)
add_test(fixedbuffer_test ${CMAKE_CURRENT_BINARY_DIR}/fixedbuffer_test)

# add_executable (logger_test logger_test.cpp)
# target_link_libraries (logger_test ccv_base)
# add_test(logger_test ${CMAKE_CURRENT_BINARY_DIR}/logger_test)

# add_executable (thread_test thread_test.cpp)
# target_link_libraries (thread_test ccv_base)
# add_test(thread_test ${CMAKE_CURRENT_BINARY_DIR}/thread_test)

# add_executable (mutex_test mutex_test.cpp)
# target_link_libraries (mutex_test ccv_base)
# add_test(mutex_test ${CMAKE_CURRENT_BINARY_DIR}/mutex_test)

# SET (UnitTest condition_test)
# add_executable (${UnitTest} ${UnitTest}.cpp)
# target_link_libraries (${UnitTest} ccv_base)
# add_test(${UnitTest} ${CMAKE_CURRENT_BINARY_DIR}/${UnitTest})

# SET (UnitTest singleton_test)
# add_executable (${UnitTest} ${UnitTest}.cpp)
# target_link_libraries (${UnitTest} ccv_base)
# add_test(${UnitTest} ${CMAKE_CURRENT_BINARY_DIR}/${UnitTest})

# SET (UnitTest keyboard_test)
# add_executable (${UnitTest} ${UnitTest}.cpp)
# target_link_libraries (${UnitTest} ccv_base)
# add_test(${UnitTest} ${CMAKE_CURRENT_BINARY_DIR}/${UnitTest})

# SET (UnitTest blocked_queue_test)
# add_executable (${UnitTest} ${UnitTest}.cpp)
# target_link_libraries (${UnitTest} ccv_base)
# add_test(${UnitTest} ${CMAKE_CURRENT_BINARY_DIR}/${UnitTest})

# SET (UnitTest compiler_test)
# add_executable (${UnitTest} ${UnitTest}.cpp)
# target_link_libraries (${UnitTest} ccv_base)
# add_test(${UnitTest} ${CMAKE_CURRENT_BINARY_DIR}/${UnitTest})

# SET (UnitTest clock_test)
# add_executable (${UnitTest} ${UnitTest}.cpp)
# target_link_libraries (${UnitTest} ccv_base)
# add_test(${UnitTest} ${CMAKE_CURRENT_BINARY_DIR}/${UnitTest})

# SET (UnitTest boost_test)
# add_executable (${UnitTest} ${UnitTest}.cpp)
# target_link_libraries (${UnitTest} ccv_base)
# add_test(${UnitTest} ${CMAKE_CURRENT_BINARY_DIR}/${UnitTest})