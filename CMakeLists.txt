cmake_minimum_required(VERSION 3.14.1)

project(glib VERSION 0.0.0 DESCRIPTION "glib archive tools")

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(lib)
add_subdirectory(test)
add_subdirectory(src)
