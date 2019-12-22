#include "frontend.hxx"

int frontend::handle(int argc, char* argv[]) {
    if (argc < 0) {
        throw std::exception();
    }
    args.clear();
    for (auto i = 0; i < argc; i++) {
        args.emplace_back(argv[i]);
    }

    try {
        ensure_arg_count(1);
        const std::string op { args[1] };

        if (op == "-l" || op == "--list") {
            list_files();
        } else if (op == "-v" || op == "--version") {
            print_version();
        } else {
            print_usage();
        }
        return 0;
    } catch (file_not_found_exception& e) {
        std::cout << e.what() << '\n';
        return 1;
    } catch (too_few_arguments_exception& e) {
        print_usage();
        return 1;
    }
}

void frontend::list_files() {
    ensure_arg_count(2);
    const std::string archive { args[2] };

    std::ifstream input_stream(archive, std::ios::binary);
    if (!input_stream) { // TODO: fs exists
        throw file_not_found_exception { archive };
    }
    for (const auto& file : lib.list(input_stream)) {
        std::cout
            << file.get_label()
            << "\t"
            << file.get_size()
            << " bytes\n";
    }
}

void frontend::print_version() const noexcept {
    std::cout
        << GLIB_VERSION_MAJOR << "."
        << GLIB_VERSION_MINOR << "."
        << GLIB_VERSION_PATCH << "\n";
}

void frontend::print_usage() const noexcept {
    std::cout << "glib, a set of tools to work with glib archives\n"
           "  usage: glib [operation] [options]\n"
           "  \n"
           "  -h --help\t\t\tprint this screen\n"
           "  -l --list\t\t\tlist files and their size in the archive\n"
           "  -v --version\t\tdisplay the version of the glib library\n";
}

void frontend::ensure_arg_count(uint32_t expected_argc) const {
    if (args.size() <= expected_argc) {
        throw too_few_arguments_exception();
    }
}
