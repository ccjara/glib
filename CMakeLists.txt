cmake_minimum_required(VERSION 3.14.1)

project(glib VERSION 0.3.0 DESCRIPTION "glib archive tools")

configure_file(lib/glib_version.hxx.in glib_version.hxx)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(lib)
add_subdirectory(test)
add_subdirectory(src)
