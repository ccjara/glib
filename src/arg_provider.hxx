#ifndef GLIB_ARG_PROVIDER_HXX
#define GLIB_ARG_PROVIDER_HXX

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "exceptions.hxx"

class arg_provider {
private:
    std::vector<std::string> args;
public:
    arg_provider(int argc, char* argv[]) {
        if (argc < 0) {
            throw std::exception();
        }
        clear();
        for (auto i = 0; i < argc; i++) {
            args.emplace_back(argv[i]);
        }
    }

    [[nodiscard]] std::string get(std::vector<std::string>::size_type index) const {
        const auto max_index { args.size() - 1 };
        if (index > max_index) {
            throw too_few_arguments_exception();
        }
        return args[index];
    }

    void clear() {
        args.clear();
    }
};


#endif
