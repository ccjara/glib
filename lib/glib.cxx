#include "glib.hxx"

std::vector<glib_file> glib::list(std::istream& input) {
    return read_files(input, true);
}

std::vector<glib_file> glib::extract(std::istream& input) {
    return read_files(input, false);
}

std::vector<glib_file> glib::read_files(std::istream& input, bool bare) {
    std::vector<glib_file> files;

    const auto archive_header { read_archive_header(input) };
    if (!archive_header.record_count) {
        return files;
    }
    files.reserve(archive_header.record_count);
    // TODO: rewrite loop to iterator
    const auto data_offset { calculate_data_offset(archive_header) };
    while(!input.eof() && input.tellg() < data_offset) {
        auto file_header { read_file_header(input) };
        glib_file file { file_header.label, file_header.size };
        if (!bare) {
            // OPTIMIZE: slower on big archives, but simpler implementation as
            // opposed to reading sequentially (headers and data are adjacent)
            const auto offset { input.tellg() };
            input.seekg(file_header.offset);
            // [OPTIMIZE] copy bytes into the output file
            std::vector<char> buffer(file_header.size);
            input.read(buffer.data(), file_header.size);
            file.set_data(std::move(buffer));
            // jump back to header to read next record
            input.seekg(offset);
        }
        files.push_back(std::move(file));
    }
    return files;
}

glib_archive_header glib::read_archive_header(std::istream& input) {
    return read_record<glib_archive_header>(input);
}

glib_file_header glib::read_file_header(std::istream& input) {
    return read_record<glib_file_header>(input);
}

std::istream::off_type glib::calculate_data_offset(
    const glib_archive_header& header
) const noexcept {
    return
        header.record_count * sizeof(glib_file_header) + sizeof(glib_archive_header);
}
