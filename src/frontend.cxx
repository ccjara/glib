#include "frontend.hxx"

bool frontend::handle(const arg_provider&& args) {
    try {
        const auto& op { args.get(1) };

        if (op == "-x" || op == "--extract") {
            extract_files(args);
        } else if (op == "-l" || op == "--list") {
            list_files(args);
        } else if (op == "-v" || op == "--version") {
            print_version();
        } else {
            print_usage();
        }
        return true;
    } catch (const file_not_found_exception& e) {
        std::cout << e.file_path << ": no such file or directory\n";
        return false;
    } catch (const too_few_arguments_exception& e) {
        print_usage();
        return false;
    }
}

void frontend::extract_files(const arg_provider& args) {
    const auto& archive { args.get(2) };
    const auto& output_path { args.get(3) };

    if (!std::filesystem::exists(archive)) {
        throw file_not_found_exception { archive };
    }
    if (!std::filesystem::is_directory(output_path)) {
        throw file_not_found_exception { output_path };
    }

    std::ifstream input_stream { archive, std::ios::binary };
    if (!input_stream) {
        throw bad_stream_exception { };
    }

    for (const auto& file : lib.extract(input_stream)) {
        std::ofstream output_stream { output_path, std::ios::binary };
        if (!output_stream) {
            throw bad_stream_exception { };
        }
        const auto file_buffer { file.get_data() };
        output_stream.write(file_buffer.data(), file_buffer.size());
    }
}

void frontend::list_files(const arg_provider& args) {
    const auto& archive { args.get(2) };
    if (!std::filesystem::exists(archive)) {
        throw file_not_found_exception { archive };
    }

    std::ifstream input_stream(archive, std::ios::binary);
    if (!input_stream) {
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
           "  -v --version\t\tdisplay the version of the glib library\n"
           "  -x --extract\t\textract all files from the archive\n";
}
