#include <catch2/catch.hpp>
#include "arg_provider.hxx"

constexpr auto TAG { "arg_provider" };

constexpr auto VALID_ARGV_SIZE { 3U };

constexpr const char* valid_argv[VALID_ARGV_SIZE] {
    "/path/to/bin",
    "-l",
    "/path/to/file"
};

// use this helper to get the argv factory param to avoid const_cast repetition
constexpr char** get_valid_argv() {
    return const_cast<char **> (valid_argv);
}

TEST_CASE("from_main", TAG) {
    SECTION("throws if too many arguments are given") {
        REQUIRE_THROWS_AS(
            arg_provider::from_main(MAX_ARGS + 1, get_valid_argv()),
            too_many_arguments_exception
        );
    }

    SECTION("throws if a negative argc is given", TAG) {
        REQUIRE_THROWS_AS(
            arg_provider::from_main(-5, get_valid_argv()),
            corrupted_args_exception
        );
    }
}

TEST_CASE("get", TAG) {
    SECTION("returns a stored program argument by index") {
        const auto provider {
            arg_provider::from_main(VALID_ARGV_SIZE, get_valid_argv())
        };
        REQUIRE(provider.get(0) == valid_argv[0]);
        REQUIRE(provider.get(1) == valid_argv[1]);
        REQUIRE(provider.get(2) == valid_argv[2]);
    }
    SECTION("throws if the requested parameter is out of bounds") {
        const auto provider {
            arg_provider::from_main(VALID_ARGV_SIZE, get_valid_argv())
        };
        REQUIRE_THROWS_AS(
            provider.get(VALID_ARGV_SIZE), // max index + 1
            too_few_arguments_exception
        );
    }
    SECTION("does not construct more argument entries than specified") {
        const auto provider { arg_provider::from_main(0, get_valid_argv()) };
        REQUIRE_THROWS_AS(provider.get(0), too_few_arguments_exception);
    }
}
