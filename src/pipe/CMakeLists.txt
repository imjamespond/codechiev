# FILE(GLOB SOURCE_FILES "*.cpp") 
# SET (Lib pipe)
# ADD_LIBRARY (learn_pipe ${Lib}.cpp) 
# TARGET_LINK_LIBRARIES (learn_pipe learn_epoll)

SET (Test test_pipe1)
ADD_EXECUTABLE (${Test} ${Test}.cpp)
TARGET_LINK_LIBRARIES (${Test} pthread rt)

SET (Test test_pipe2)
ADD_EXECUTABLE (${Test} ${Test}.cpp)
TARGET_LINK_LIBRARIES (${Test} pthread rt)


SET (Test test_pipe3)
ADD_EXECUTABLE (${Test} ${Test}.cpp)
TARGET_LINK_LIBRARIES (${Test} learn_thread)

SET (Test test_pipe4)
ADD_EXECUTABLE (${Test} ${Test}.cpp)
TARGET_LINK_LIBRARIES (${Test} learn_epoll)


# ADD_SUBDIRECTORY(tests)