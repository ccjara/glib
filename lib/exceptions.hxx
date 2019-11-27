#ifndef GLIB_EXCEPTIONS_HXX
#define GLIB_EXCEPTIONS_HXX

#include <exception>

class bad_stream_exception : public std::exception {
    [[nodiscard]] const char* what() const noexcept override {
        return "Bad Stream";
    }
};

class end_of_stream_exception : public std::exception {
    [[nodiscard]] const char* what() const noexcept override {
        return "Stream has ended before the requested sequence length was read";
    }
};

#endif
