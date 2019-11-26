#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

constexpr uint16_t MAX_RECORD_SIZE { 64000 };

struct glib_record {
    uint32_t offset; // absolute byte offset of the raw data
    char label[22]; // zero padded label
    uint16_t size; // size in bytes

    glib_record() : offset(0), label(""), size(0) {}
};

// glib_header is a re-purposed (abstract) glib_record with a 'GLIB_FILE' label
// and fixed zero size which we are not going to use
struct glib_header {
    uint32_t record_count;
    glib_header() : record_count(0), unused("") {}
private:
    char unused[sizeof(glib_record::label) + sizeof(glib_record::size)];
};

static_assert(sizeof(glib_header) == sizeof(glib_record));
static_assert(sizeof(glib_record) == 28);

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
    glib_header header;
    input_stream.read(reinterpret_cast<char *> (&header), sizeof(header));
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

        if (!std::filesystem::exists(dir)) {
            if (!std::filesystem::create_directory(dir)) {
                std::cout
                    << "Could not create target directory"
                    << dir
                    << " to extract the files in\n";
            }
        }
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