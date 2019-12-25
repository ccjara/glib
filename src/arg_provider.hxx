#ifndef GLIB_ARG_PROVIDER_HXX
#define GLIB_ARG_PROVIDER_HXX

#include <string>
#include <vector>

#include "frontend_exception.hxx"

// program argument container which abstracts the low level main argc, argv
// and integrates with frontend exceptions when doing boundary checking

constexpr size_t MAX_ARGS { 255U };

class arg_provider {
private:
    std::vector<std::string> args;
public:
    static arg_provider from_main(int argc, char* argv[]) {
        if (argc < 0 || argv == nullptr) {
            throw corrupted_args_exception { };
        }
        const auto arg_count { static_cast<size_t> (argc) };
        if (argc > MAX_ARGS) {
            throw too_many_arguments_exception { arg_count, MAX_ARGS };
        }
        arg_provider provider;

        for (auto i = 0; i < arg_count; i++) {
            provider.args.emplace_back(argv[i]);
        }
        return provider;
    }

    [[nodiscard]] const std::string& get(size_t index) const {
        if (index + 1 > args.size()) {
            throw too_few_arguments_exception { index, args.size() };
        }
        return args[index];
    }
};


#endif
