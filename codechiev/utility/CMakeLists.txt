message(STATUS "compiling codechief::utility current_source_dir: " ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB OPENSSL_SOURCE "openssl/*.cpp")
set (LIB codechief_utility)
add_library (${LIB} ${OPENSSL_SOURCE})#add source to this lib
target_link_libraries (${LIB} codechief_base pthread rt ssl crypto)#link for this lib, rt is for get_clock

message(STATUS "message please run \"make install\" : " ${CMAKE_INSTALL_PREFIX})
install(TARGETS ${LIB} DESTINATION ${ROOT}/libs)
file(GLOB HEADERS "*.h" "*.hpp")
install(FILES ${HEADERS} DESTINATION ${ROOT}/include/codechiev/utility)

enable_testing()
set (BASE_HEADERS ${CMAKE_CURRENT_SOURCE_DIR})
set (MY_LIBS ${LIB})
add_subdirectory(tests)

