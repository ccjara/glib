#ifndef GLIB_FRONTEND_EXCEPTION_HXX
#define GLIB_FRONTEND_EXCEPTION_HXX

#include <exception>

class frontend_exception : public std::exception {
};

class file_not_found_exception : public frontend_exception {
public:
    const std::string file_path;

    explicit file_not_found_exception(std::string path)
        : file_path(std::move(path)) {
    }
};

class corrupted_args_exception : public frontend_exception {
};

class too_few_arguments_exception : public frontend_exception {
public:
    const size_t arg_number; // requested arg number (not index)
    const size_t max_args; // argument count in container

    too_few_arguments_exception(size_t arg_number, size_t max_args) :
        arg_number(arg_number),
        max_args(max_args) {
    }
};

class too_many_arguments_exception : public frontend_exception {
public:
    const size_t arg_count; // number of arguments (i.e. amount, not as index)
    const size_t max_args; // max allowed arguments

    too_many_arguments_exception(size_t arg_count, size_t max_args) :
        arg_count(arg_count),
        max_args(max_args) {
    }
};

#endif
