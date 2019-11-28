#include <fstream>
#include <iostream>
#include <string>
// #include <filesystem>
#include "glib.hxx"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage: glibex <path_to_glib_file>\n";
        return 0;
    }
    std::string archive { argv[1] };
    std::ifstream input_stream(archive, std::ios::binary);
    if (!input_stream) {
        std::cout << "Could not open input file " << archive << "\n";
        return 1;
    }

    glib lib;
    for (const auto& file : lib.list(input_stream)) {
        std::cout << file.get_label() << '\n';
    }
    return 0;
}
