# FILE(GLOB SOURCE_FILES "*.cpp")
# ADD_LIBRARY (cc_base ${SOURCE_FILES})
SET (Test test_pipe1)
ADD_EXECUTABLE (${Test} ${Test}.cpp)
TARGET_LINK_LIBRARIES (${Test} pthread rt)

SET (Test test_pipe2)
ADD_EXECUTABLE (${Test} ${Test}.cpp)
TARGET_LINK_LIBRARIES (${Test} pthread rt)


SET (Test test_pipe3)
ADD_EXECUTABLE (${Test} ${Test}.cpp)
TARGET_LINK_LIBRARIES (${Test} learn_thread)

# ADD_SUBDIRECTORY(tests)