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

    // the header record acts as a dummy record with
    // the offset being the file count in the archive
    auto header { read_header(input_stream) };
    if (header.record_count == 0) {
        std::cout << "Archive is empty\n";
        return 0;
    }
    std::cout << "Number of files in archive: " << header.record_count << "\n";

    const std::string dir { archive + "_files" };
    std::cout << "Extracting to: " << dir << "\n";

    for (uint32_t i { 0 }; i < header.record_count; i++) {
        // OPTIMIZE: does not read glib archive sequentially but jumps
        // around which could get problematic on big file archives
        glib_record record;
        input_stream.read(reinterpret_cast<char *> (&record), sizeof(record));

        if (record.size == 0) {
            std::cout << "Skipping empty record '" << record.label << "\n";
            continue;
        } else if (record.size > MAX_RECORD_SIZE) {
            std::cout
                << "Corrupted record '"
                << record.label
                << "': unexpected record size\n";
            return 1;
        }
        std::cout << record.label << " (" << record.size << " bytes)\n";

        /*
        if (!std::filesystem::exists(dir)) {
            if (!std::filesystem::create_directory(dir)) {
                std::cout
                    << "Could not create target directory"
                    << dir
                    << " to extract the files in\n";
            }
        }
        */

        std::ofstream output_stream(dir + "\\" + record.label, std::ios::binary);
        // jump to first byte of glib record
        const auto offset { input_stream.tellg() };
        input_stream.seekg(record.offset);
        // [OPTIMIZE] copy bytes into the output file
        auto data { new char[record.size] };
        input_stream.read(data, record.size);
        output_stream.write(data, record.size);
        delete[] data;
        // jump back to header to read next record
        input_stream.seekg(offset);
    }
    std::cout << "Done.\n";
    return 0;
}
