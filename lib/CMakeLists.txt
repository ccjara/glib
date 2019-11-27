add_library(glib_static STATIC
    glib.cxx
    glib_file.cxx
    glib_file_validator.cxx
    exceptions.hxx
)

set_target_properties(glib_static PROPERTIES LINKER_LANGUAGE CXX)

set_target_properties(glib_static PROPERTIES VERSION ${PROJECT_VERSION})
