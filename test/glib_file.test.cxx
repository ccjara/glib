#include <catch2/catch.hpp>

#include "glib_file_validator.hxx"

constexpr auto TAG { "glib_file" };

TEST_CASE("initializes correctly", TAG) {
    const glib_file f { "label", { 'D', 'A', 'T', 'A' } };
    REQUIRE(f.get_label() == "label");

    const auto& container { f.get_container() };
    std::string str(container.cbegin(), container.cend());

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
    glib_file f { "label", { 'D', 'A', 'T', 'A' } };

    f.set_container({ 'A', 'B', 'C' });
    const auto container { f.get_container() };
    std::string str(container.cbegin(), container.cend());
    REQUIRE(str == "ABC");
}

TEST_CASE("does not have data if the data container is empty", TAG) {
    const glib_file f { "label" };
    REQUIRE(!f.has_data());
    glib_file ff { "label", std::vector<char> { 'D' } };
    REQUIRE(ff.has_data());
    ff.set_container({});
    REQUIRE(!ff.has_data());
}

TEST_CASE("does not have a label if the label container is empty", TAG) {
    glib_file f { "label" };
    REQUIRE(f.has_label());
    f.rename("");
    REQUIRE(!f.has_label());
}

TEST_CASE("can be assigned size information without allocating data", TAG) {
    glib_file f { "label", 77777 };
    REQUIRE(f.get_size() == 77777);
    REQUIRE(!f.has_data());

    f.set_size(50);

    REQUIRE(f.get_size() == 50);
    REQUIRE(!f.has_data());
}

TEST_CASE("has zero size by default", TAG) {
    glib_file f { "label" };
    REQUIRE(f.get_size() == 0);
    REQUIRE(!f.has_data());
}

TEST_CASE("overwrites the size information if data is assigned", TAG) {
    glib_file f { "label", 16 };
    f.set_container({ 'D', 'A', 'T', 'A' });
    REQUIRE(f.get_size() == 4);
}

TEST_CASE("assigns the size from the data container if given", TAG) {
    glib_file f { "label", { 'D', 'A', 'T', 'A' } };
    REQUIRE(f.get_size() == 4);
    REQUIRE(f.get_container().size() == 4);
}
