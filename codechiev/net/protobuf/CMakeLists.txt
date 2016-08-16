message(STATUS "compiling codechief::net current_source_dir: " ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_command(OUTPUT test.pb.cc test.pb.h
COMMAND protoc
ARGS --cpp_out . ${CMAKE_CURRENT_SOURCE_DIR}/test.proto -I${CMAKE_CURRENT_SOURCE_DIR}
DEPENDS test.proto)
add_custom_target(proto_out DEPENDS test.pb.cc test.pb.h)

file(GLOB SOURCE "*.cpp")
set (LIB codechiev_net_proto)
add_library (${LIB} ${SOURCE})#add source to this lib
add_dependencies(${LIB} proto_out)
target_link_libraries (${LIB} codechief_base codechief_net pthread rt protobuf)#link for this lib, rt is for get_clock



message(STATUS "message please run \"make install\" : " ${CMAKE_INSTALL_PREFIX})
install(TARGETS ${LIB} DESTINATION ${ROOT}/libs)
file(GLOB HEADERS "*.h" "*.hpp")
install(FILES ${HEADERS} DESTINATION ${ROOT}/include/codechiev/net/protobuf)

enable_testing()
set (MY_LIBS ${LIB})
add_subdirectory(tests)
