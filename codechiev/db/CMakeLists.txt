message(STATUS "compiling codechief::net current_source_dir: " ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB SOURCE "*.cpp")
set (LIB codechief_db)
add_library (${LIB} ${SOURCE})#add source to this lib
target_link_libraries (${LIB} pq codechief_base pthread rt)#link for this lib, rt is for get_clock

message(STATUS "message please run \"make install\" : " ${CMAKE_INSTALL_PREFIX})
install(TARGETS ${LIB} DESTINATION ${ROOT}/libs)
file(GLOB HEADERS "*.h" "*.hpp")
install(FILES ${HEADERS} DESTINATION ${ROOT}/include/codechiev/db)

enable_testing()
set (BASE_HEADERS ${CMAKE_CURRENT_SOURCE_DIR})
set (MY_LIBS ${LIB})
add_subdirectory(tests)

