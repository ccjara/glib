#include <fstream>
#include <iostream>
#include <string>
// #include <filesystem>
#include "glib.hxx"
#include "frontend.hxx"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << get_usage();
        return 0;
    }
    const std::string op { argv[1] };
    if (op == "-v" || op == "--version") {
        std::cout
            << GLIB_VERSION_MAJOR << "."
            << GLIB_VERSION_MINOR << "."
            << GLIB_VERSION_PATCH << "\n";
        return 0;
    }
    if (argc < 3) {
        std::cout << get_usage();
        return 0;
    }
    const std::string archive { argv[2] };
    std::ifstream input_stream(archive, std::ios::binary);
    if (!input_stream) {
        std::cout << "Could not read archive " << archive << "\n";
        return 1;
    }
    glib lib;
    if (op == "-l" || op == "--list") {
        for (const auto& file : lib.list(input_stream)) {
            std::cout
                << file.get_label()
                << "\t"
                << file.get_size()
                << " bytes\n";
        }
        return 0;
    }
    std::cout << get_usage();
    return 0;
}
