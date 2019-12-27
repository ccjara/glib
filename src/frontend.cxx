#include "frontend.hxx"

frontend::frontend(std::ostream& output_stream, std::ostream& error_stream) :
    out(output_stream),
    err(error_stream) {
}

bool frontend::handle(const arg_provider& args) {
    try {
        const auto& op { args.get(0) };

        if (op == "c" || op == "--create") {
            create_archive(args);
        } else if (op == "-x" || op == "--extract") {
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
        err << e.file_path << ": no such file or directory\n";
        return false;
    } catch (const too_few_arguments_exception& e) {
        print_usage();
        return false;
    } catch (const output_path_missing_exception& e) {
        err << "Argument 'output path' (-o, --output) missing\n";
    } catch (const too_many_arguments_exception& e) {
        err
            << "Too many arguments. Can only handle up to"
            << e.max_args
            << " arguments (" << e.arg_count << " given).\n";
        return false;
    }
}

// glib -c in_f1 in_f2 in_f3 -o output_path
void frontend::create_archive(const arg_provider& args) {
    constexpr auto MIN_ARGS_REQUIRED { 4 };
    const auto arg_size { args.size() };
    if (arg_size < MIN_ARGS_REQUIRED) {
        throw too_few_arguments_exception { arg_size, MIN_ARGS_REQUIRED };
    }

    std::string output_path;
    std::vector<glib_file> files(arg_size);

    for (auto it { args.cbegin() }; it != args.cend(); it++) {
        if (*it == "-o" || *it == "--output") {
            if (++it == args.cend()) {
                break;
            }
            output_path = *it;
        } else {
            std::string file_path { *it };
            if (!std::filesystem::exists(file_path)) {
                throw file_not_found_exception { std::move(file_path) };
            }
            const auto size { std::filesystem::file_size(file_path) };
            std::ifstream input { file_path, std::ios::binary };
            if (!input) {
                throw bad_stream_exception {};
            }

            std::vector<char> data_v(size);
            input.read(data_v.data(), size);
            files.emplace_back(std::move(file_path), std::move(data_v));
        }
    }

    if (!output_path.length()) {
        throw output_path_missing_exception { };
    }
    if (!std::filesystem::is_directory(output_path)) {
        throw invalid_directory_exception { std::move(output_path) };
    }

    std::ofstream output_stream { output_path, std::ios::binary };
    if (!output_stream) {
        throw directory_not_writable_exception { std::move(output_path) };
    }
    lib.create(files, output_stream);
}

void frontend::extract_files(const arg_provider& args) {
    const auto& archive { args.get(1) };
    const auto& output_path { args.get(2) };

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
        std::ofstream file_output_stream { output_path, std::ios::binary };
        if (!file_output_stream) {
            throw bad_stream_exception { };
        }
        const auto& file_buffer { file.get_data() };
        file_output_stream.write(file_buffer.data(), file_buffer.size());
    }
}

void frontend::list_files(const arg_provider& args) {
    const auto& archive { args.get(1) };
    if (!std::filesystem::exists(archive)) {
        throw file_not_found_exception { archive };
    }

    std::ifstream input_stream { archive, std::ios::binary };
    if (!input_stream) {
        throw bad_stream_exception { };
    }
    for (const auto& file : lib.list(input_stream)) {
        out << file.get_label() << "\t" << file.get_size() << " bytes\n";
    }
}

void frontend::print_version() const {
    out
        << GLIB_VERSION_MAJOR << "."
        << GLIB_VERSION_MINOR << "."
        << GLIB_VERSION_PATCH << "\n";
}

void frontend::print_usage() const {
    out << "glib, a set of tools to work with glib archives\n"
           "  usage: glib [operation] [options]\n"
           "  \n"
           "  -c --create\t\tcreate an archive from the given files\n"
           "  -h --help\t\t\tprint this screen\n"
           "  -l --list\t\t\tlist files and their size in the archive\n"
           "  -v --version\t\tdisplay the version of the glib library\n"
           "  -x --extract\t\textract all files from the archive\n";
}
