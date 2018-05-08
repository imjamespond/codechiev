

ADD_EXECUTABLE("simple-http-client" "simple-http-client.cpp")
ADD_EXECUTABLE("libevent-rot13-server" "libevent-rot13-server.cpp")

TARGET_LINK_LIBRARIES("libevent-rot13-server" event)