#ifndef GLIB_GLIB_EXCEPTION_HXX
#define GLIB_GLIB_EXCEPTION_HXX

#include <exception>

class glib_exception : public std::exception {
};

class bad_stream_exception : public glib_exception {
public:
    [[nodiscard]] const char* what() const noexcept override {
        return "Bad stream";
    }
};

class end_of_stream_exception : public glib_exception {
public:
    [[nodiscard]] const char* what() const noexcept override {
        return "Stream has ended before the requested sequence length was read";
    }
};

#endif
