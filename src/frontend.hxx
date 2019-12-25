#ifndef GLIB_FRONTEND_HXX
#define GLIB_FRONTEND_HXX

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "arg_provider.hxx"
#include "glib_exception.hxx"
#include "glib.hxx"

class frontend {
private:
    glib lib;

    void extract_files(const arg_provider& args);
    void list_files(const arg_provider& args);
    void print_usage() const noexcept;
    void print_version() const noexcept;
public:
    bool handle(const arg_provider&& args);
};

#endif
