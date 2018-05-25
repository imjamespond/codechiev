

ADD_EXECUTABLE("simple-http-client" "simple-http-client.cpp")
ADD_EXECUTABLE("libevent-rot13-server" "libevent-rot13-server.cpp")

ADD_EXECUTABLE("example-1" "example-1.cpp")
ADD_EXECUTABLE("hello-world" "hello-world.c")
ADD_EXECUTABLE("time-test" "time-test.c")

TARGET_LINK_LIBRARIES("libevent-rot13-server" event)
TARGET_LINK_LIBRARIES("example-1" event pthread)
TARGET_LINK_LIBRARIES("hello-world" event)
TARGET_LINK_LIBRARIES("time-test" event)