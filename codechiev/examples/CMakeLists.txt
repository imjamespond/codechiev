

# ADD_EXECUTABLE("simple-http-client" "simple-http-client.cpp")
# ADD_EXECUTABLE("libevent-rot13-server" "libevent-rot13-server.cpp")

# ADD_EXECUTABLE("example-1" "example-1.cpp")
# ADD_EXECUTABLE("example-timer" "example-timer.cpp")
# ADD_EXECUTABLE("hello-world" "hello-world.c")
# ADD_EXECUTABLE("time-test" "time-test.cpp")

# TARGET_LINK_LIBRARIES("libevent-rot13-server" event event_pthreads ccv_base)
# TARGET_LINK_LIBRARIES("example-1" event ccv_base)
# TARGET_LINK_LIBRARIES("example-timer" pthread event event_pthreads ccv_base)
# TARGET_LINK_LIBRARIES("hello-world" event)
# TARGET_LINK_LIBRARIES("time-test" event ccv_base)

ADD_EXECUTABLE("chat-room-server" 
               "chat-room/chat_room_server.cpp" 
               "chat-room/chat_room_client.cpp" 
               "chat-room/chat_room.cpp")
TARGET_LINK_LIBRARIES("chat-room-server" ccv_ev)
 