add_library(glib_static STATIC
    glib.cxx
    glib_defs.hxx
    glib_exception.hxx
    glib_file.cxx
    glib_file_validator.cxx
    glib_version.hxx
)

target_include_directories(glib_static PUBLIC "${PROJECT_BINARY_DIR}")

set_target_properties(glib_static PROPERTIES LINKER_LANGUAGE CXX)

set_target_properties(glib_static PROPERTIES VERSION ${PROJECT_VERSION})
