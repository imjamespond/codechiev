message(STATUS "message current_source_dir: " ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB SOURCE "*.cpp")
set (LIB codechief_base)
add_library (${LIB} ${SOURCE})#add source to this lib
target_link_libraries (${LIB} pthread rt)#link for this lib, rt is for get_clock

#add_library(muduo_base_cpp11 ${SOURCE})
#target_link_libraries(muduo_base_cpp11 pthread rt)
#set_target_properties(muduo_base_cpp11 PROPERTIES COMPILE_FLAGS "-std=c++0x")


message(STATUS "message please run \"make install\" : " ${CMAKE_INSTALL_PREFIX})
install(TARGETS ${LIB} DESTINATION ${ROOT}/libs)
file(GLOB HEADERS "*.h" "*.hpp")
install(FILES ${HEADERS} DESTINATION ${ROOT}/include/codechiev/base)

enable_testing()
set (BASE_HEADERS ${CMAKE_CURRENT_SOURCE_DIR})
set (MY_LIBS ${LIB})
add_subdirectory(tests)

