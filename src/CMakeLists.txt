add_executable(
    glib_bin
    frontend.hxx
    main.cxx
)

target_link_libraries(glib_bin glib_static)

target_include_directories(glib_bin PUBLIC ${CMAKE_SOURCE_DIR}/lib)
