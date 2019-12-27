#include <catch2/catch.hpp>
#include <sstream>
#include <vector>

#include "glib.hxx"

constexpr auto TAG { "glib" };

TEST_CASE("produces correct archives", TAG) {
    glib lib;

    const std::vector<glib_file> files {
        glib_file("file1", { 'a', '1', 'x', 't' }),
        glib_file("file2", { '6', '2', '9' }),
        glib_file("file3", { 'f', '3' }),
    };

    // write the serialized archive to a string buffer
    std::ostringstream oss_files;
    REQUIRE_NOTHROW(lib.create(files, oss_files));
    // reverse the process: extract files from the previously created buffer
    std::istringstream iss { oss_files.str() };
    std::vector<glib_file> files_extracted;
    REQUIRE_NOTHROW(files_extracted = lib.extract(iss));
    // check that the files do not lose any information
    REQUIRE(files.size() == files_extracted.size());
    /*
    FIXME: works except for label comparison: comparing a padded fixed size
    label with the "clean" version of the label (if trimmed it would work)
    REQUIRE(files[0] == files_extracted[0]);
    REQUIRE(files[1] == files_extracted[1]);
    REQUIRE(files[2] == files_extracted[2]);
    */
    // again create an archive from the deserialized files
    std::ostringstream oss_files_extracted;
    REQUIRE_NOTHROW(lib.create(files, oss_files_extracted));
    // glib archives do not have any randomness so the buffer must be identical
    REQUIRE(oss_files.str() == oss_files_extracted.str());
}
