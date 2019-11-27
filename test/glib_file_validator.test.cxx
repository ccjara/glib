#include <catch2/catch.hpp>
#include <memory>
#include "glib_file.hxx"
#include "glib_file_validator.hxx"

constexpr auto tag { "glib_file_validator" };

constexpr uint32_t len(const char* str) {
    return *str ? 1 + len(str + 1) : 0;
}

TEST_CASE("returns no_label if the file has no label", tag) {
    const glib_file_validator v;
    glib_file f;
    f.set_data({ 'H', 'E', 'L', 'L', 'O' });
    REQUIRE(v.validate(f) == glib_file_validation_result::no_label);
}

TEST_CASE("returns label_too_long if the label exceeds the max length", tag) {
    constexpr auto s {
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
    };
    static_assert(len(s) > MAX_LABEL_LENGTH);

    const glib_file_validator v;
    const glib_file f(s, { 'H', 'E', 'L', 'L', 'O' });

    REQUIRE(v.validate(f) == glib_file_validation_result::label_too_long);
}

TEST_CASE("returns no_data if the file has no body", tag) {
    const glib_file_validator v;
    const glib_file f("hello");

    REQUIRE(v.validate(f) == glib_file_validation_result::no_data);
}

TEST_CASE("returns data_too_large if the data exceeds the max size", tag) {
    // FIXME: dangerous way of testing - need to introduce a seam to mock
    // the size so that the test does not allocate anything
    std::vector<char> super_vector(MAX_RECORD_SIZE + 1, 0);
    const glib_file_validator v;
    const glib_file f("hello", std::move(super_vector));

    REQUIRE(v.validate(f) == glib_file_validation_result::data_too_large);
}


TEST_CASE("returns ok if no errors were found", tag) {
    const glib_file_validator v;
    const glib_file f("hello", { 'H', 'E', 'L', 'L', 'O' });

    REQUIRE(v.validate(f) == glib_file_validation_result::ok);
}
