add_executable(
    glib_test
    arg_provider.test.cxx
    glib.test.cxx
    glib_file.test.cxx
    glib_file_validator.test.cxx
    main.cxx
)

find_package(Catch2 REQUIRED)

target_link_libraries(glib_test Catch2::Catch2 glib_static)
target_include_directories(glib_test PUBLIC ${CMAKE_SOURCE_DIR}/lib)
target_include_directories(glib_test PUBLIC ${CMAKE_SOURCE_DIR}/src)

include(CTest)
include(Catch)

add_test(NAME glib_test COMMAND glib_test -o report.xml)
