message("message compile and link test...")

set(CPP_SUFFIX ".cpp")
file(GLOB CPP *${CPP_SUFFIX})
foreach(file ${CPP})
get_filename_component(filename ${file} NAME_WE)
message(STATUS ${filename}${CPP_SUFFIX})

include_directories (${BASE_HEADERS})
add_executable (${filename} ${filename}${CPP_SUFFIX})
target_link_libraries (${filename} ${MY_LIBS} pcre)
add_test(${filename} ${CMAKE_CURRENT_BINARY_DIR}/${filename})

endforeach(file)




