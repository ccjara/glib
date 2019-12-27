#ifndef GLIB_GLIB_EXCEPTION_HXX
#define GLIB_GLIB_EXCEPTION_HXX

#include <exception>

#include "glib_file_validator.hxx"

class glib_exception : public std::exception {
};

class invalid_file_exception : public glib_exception {
public:
    const glib_file file_bare;
    const glib_file_validation_result validation_result;

    explicit invalid_file_exception(
        const glib_file& file,
        glib_file_validation_result res
    ) :
        file_bare(std::move(glib_file(file, glib_file::copy_bare_tag {}))),
        validation_result(res) {
    }
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
