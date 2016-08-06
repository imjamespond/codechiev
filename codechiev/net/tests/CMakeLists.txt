message("message compile and link test...")

add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/test.pb.cc ${CMAKE_CURRENT_SOURCE_DIR}/test.pb.h
  COMMAND protoc
  ARGS --cpp_out . ${CMAKE_CURRENT_SOURCE_DIR}/test.proto -I${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS test.proto)
add_custom_target(proto_out DEPENDS test.pb.cc test.pb.h)

set(CPP_SUFFIX ".cpp")
file(GLOB CPP *${CPP_SUFFIX} test.pb.cc test.pb.h)
foreach(file ${CPP})
get_filename_component(filename ${file} NAME_WE)
message(STATUS ${filename}${CPP_SUFFIX})

include_directories (${BASE_HEADERS})
add_executable (${filename} ${filename}${CPP_SUFFIX})
target_link_libraries (${filename} ${MY_LIBS} pcre protobuf)
add_dependencies(${filename} proto_out)
add_test(${filename} ${CMAKE_CURRENT_BINARY_DIR}/${filename})

endforeach(file)




