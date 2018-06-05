
FILE(GLOB SOURCE_FILES "*.cpp")
ADD_LIBRARY (ccv_ev ${SOURCE_FILES})
TARGET_LINK_LIBRARIES (ccv_ev ccv_base event event_pthreads)

ADD_SUBDIRECTORY(tests)
