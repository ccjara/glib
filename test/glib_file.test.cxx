#include <catch2/catch.hpp>
#include <algorithm>

#include "glib_file_validator.hxx"

constexpr auto TAG { "glib_file" };

TEST_CASE("initializes correctly", TAG) {
    const glib_file f { "label", {'D', 'A', 'T', 'A' } };
    REQUIRE(f.get_label() == "label");

    const auto data { f.get_data() };
    std::string str(data.cbegin(), data.cend());

    REQUIRE(str == "DATA");
}

TEST_CASE("can be renamed", TAG) {
    glib_file f { "old" };

    f.rename("new");
    REQUIRE(f.get_label() == "new");
    f.rename(std::string("very new"));
    REQUIRE(f.get_label() == "very new");
}

TEST_CASE("can be assigned new data", TAG) {
    glib_file f { "label", {'D', 'A', 'T', 'A' } };

    f.set_data({ 'A', 'B', 'C' });
    const auto data { f.get_data() };
    std::string str(data.cbegin(), data.cend());
    REQUIRE(str == "ABC");
}

TEST_CASE("does not have data if the data container is empty", TAG) {
    const glib_file f { "label" };
    REQUIRE(!f.has_data());
    glib_file ff { "label", { 'D' } };
    REQUIRE(ff.has_data());
    ff.set_data({});
    REQUIRE(!ff.has_data());
}

TEST_CASE("does not have a label if the label container is empty", TAG) {
    glib_file f { "label" };
    REQUIRE(f.has_label());
    f.rename("");
    REQUIRE(!f.has_label());
}
