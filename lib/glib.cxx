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

        glib_file file {
            std::string { file_header.label.data(), FIXED_LABEL_SIZE },
            file_header.size
        };
        if (!bare) {
            // OPTIMIZE: slower on big archives, but simpler implementation as
            // opposed to reading sequentially (headers and data are adjacent)
            const auto offset { input.tellg() };
            input.seekg(file_header.offset);
            // OPTIMIZE: copy bytes into the output file
            std::vector<char> buffer(file_header.size);
            input.read(buffer.data(), file_header.size);
            file.set_container(std::move(buffer));
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

constexpr std::istream::off_type glib::calculate_data_offset(
    const glib_archive_header& header,
    const size_t global_offset
) const noexcept {
    return global_offset
        + header.record_count * sizeof(glib_file_header)
        + sizeof(glib_archive_header);
}

void glib::create(const std::vector<glib_file>& files, std::ostream& output) {
    if (!output) {
        throw bad_stream_exception {};
    }
    const auto file_count { files.size() };
    const glib_archive_header header { static_cast<uint32_t> (files.size()) };

    output.write(reinterpret_cast<const char *> (&header), sizeof(header));

    for (size_t i { 0U }, data_offset { 0U }; i < file_count; i++) {
        using v = glib_file_validation_result;

        const auto& file { files[i] };
        const auto validation_result { glib_file_validator::validate(file) };
        if (validation_result != v::ok) {
            throw invalid_file_exception { std::move(file), validation_result };
        }
        const auto& label { file.get_label() };

        glib_file_header file_header;
        std::copy(label.cbegin(), label.cend(), file_header.label.begin());
        file_header.size = file.get_size();
        file_header.offset = calculate_data_offset(header, data_offset);
        data_offset += file.get_size();

        output.write(
            reinterpret_cast<const char *> (&file_header),
            sizeof(file_header)
        );
    }

    for (const auto& file: files) {
        output.write(file.get_container().data(), file.get_size());
    }
}
