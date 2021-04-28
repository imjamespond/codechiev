# FILE(GLOB SOURCE_FILES "*.cpp")
# ADD_LIBRARY (cc_base ${SOURCE_FILES})
SET (Test test1)
ADD_EXECUTABLE (${Test} ${Test}.cpp)
TARGET_LINK_LIBRARIES (${Test} pthread rt)

SET (Test test2)
ADD_EXECUTABLE (${Test} ${Test}.cpp)
TARGET_LINK_LIBRARIES (${Test} pthread rt)

# ADD_SUBDIRECTORY(tests)