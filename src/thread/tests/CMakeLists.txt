
FILE(GLOB SOURCE "*.cpp")

foreach(file ${SOURCE})
  get_filename_component(filename ${file} NAME_WE) 
  message(STATUS ${filename})

  add_executable (${filename} ${filename}.cpp)
  target_link_libraries (${filename} learn_thread pthread rt)
  add_test(${filename} ${CMAKE_CURRENT_BINARY_DIR}/${filename})
endforeach(file)
