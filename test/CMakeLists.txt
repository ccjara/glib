add_executable(
    glib_test
    main.cxx
    glib.test.cxx
)

find_package(Catch2 REQUIRED)

target_link_libraries(glib_test Catch2::Catch2)
target_include_directories(glib_test PUBLIC ${CMAKE_SOURCE_DIR}/lib)

include(CTest)
include(Catch)

add_test(NAME glib_test COMMAND glib_test -o report.xml)
