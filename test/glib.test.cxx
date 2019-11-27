#include <catch2/catch.hpp>
#include <istream>
#include "glib.hxx"
#include "exceptions.hxx"

TEST_CASE("throws bad_stream_exception if the given input stream is bad", "read_header") {
    class error_stream : public std::istream {
        bool bad() const noexcept {
            return true;
        }
    };
    error_stream stream;
    REQUIRE_THROWS_AS(read_header(stream), bad_stream_exception);
}
