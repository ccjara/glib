#ifndef GLIB_ARG_PROVIDER_HXX
#define GLIB_ARG_PROVIDER_HXX

#include <string>
#include <vector>

#include "frontend_exception.hxx"

// program argument container which abstracts the low level main argc, argv
// and integrates with frontend exceptions when doing boundary checking

constexpr size_t MAX_ARGS { 255U };

class arg_provider {
    using container_t = std::vector<std::string>;

private:
    std::string path_to_script;

    container_t args;
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

        provider.path_to_script = argv[0];
        // only store explicit user arguments
        for (auto i = 1; i < arg_count; i++) {
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

    [[nodiscard]] container_t::size_type size() const noexcept {
        return args.size();
    }

    [[nodiscard]] container_t::const_iterator cbegin() const noexcept {
        return args.cbegin();
    }

    [[nodiscard]] container_t::const_iterator cend() const noexcept {
        return args.cend();
    }

    [[nodiscard]] const std::string& get_path_to_script() const noexcept {
        return path_to_script;
    }
};


#endif
