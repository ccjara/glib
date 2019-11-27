add_executable(glib_bin main.cxx ../lib/glib.cxx ../lib/glib.hxx)

target_link_libraries(glib_bin glib_static)

target_include_directories(glib_bin PUBLIC ${CMAKE_SOURCE_DIR}/lib)
