#ifndef GLIB_EXCEPTIONS_HXX
#define GLIB_EXCEPTIONS_HXX

#include <exception>
#include <utility>

class file_not_found_exception : public std::exception {
protected:
    std::string what_str;

public:
    explicit file_not_found_exception(const std::string& path) {
        what_str = "File " + path + " not found\n";
    }

    [[nodiscard]] const char* what() const noexcept override {
        return what_str.c_str();
    }
};

class too_few_arguments_exception : public std::exception {
public:
    [[nodiscard]] const char* what() const noexcept override {
        return "Too few arguments";
    }
};

class bad_stream_exception : public std::exception {
public:
    [[nodiscard]] const char* what() const noexcept override {
        return "Bad stream";
    }
};

class end_of_stream_exception : public std::exception {
public:
    [[nodiscard]] const char* what() const noexcept override {
        return "Stream has ended before the requested sequence length was read";
    }
};

#endif
