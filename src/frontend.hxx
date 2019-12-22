#ifndef GLIB_FRONTEND_HXX
#define GLIB_FRONTEND_HXX

#include <cstdint>
// #include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "exceptions.hxx"
#include "glib.hxx"

class frontend {
private:
    glib lib;

    std::vector<std::string> args;

    void list_files();
    void print_usage() const noexcept;
    void print_version() const noexcept;

    void ensure_arg_count(uint32_t expected_argc) const;
public:
    int handle(int argc, char* argv[]);
};

#endif
