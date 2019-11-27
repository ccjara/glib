#include "glib.hxx"

std::vector<glib_file> glib::list(std::istream& input) {
    std::vector<glib_file> container;

    const auto archive_header { read_archive_header(input) };
    if (!archive_header.record_count) {
        return container;
    }
    // TODO: rewrite loop to iterator
    const auto data_offset { calculate_data_offset(archive_header) };
    while(input.tellg() <= data_offset) {
        glib_file file(read_file_header(input).label);
        container.emplace_back(std::move(file));
    }
    return container;
}

glib_archive_header glib::read_archive_header(std::istream &input) {
    return read_record<glib_archive_header>(input);
}

glib_file_header glib::read_file_header(std::istream &input) {
    return read_record<glib_file_header>(input);
}

std::istream::off_type glib::calculate_data_offset(
    const glib_archive_header &header
) const noexcept {
    return
        header.record_count * sizeof(glib_file_header) + sizeof(glib_archive_header);
}
