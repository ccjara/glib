#ifndef GLIB_GLIB_HXX
#define GLIB_GLIB_HXX

#include <istream>
#include <optional>
#include <vector>

#include "glib_defs.hxx"
#include "glib_file.hxx"
#include "glib_version.hxx"
#include "glib_exception.hxx"

class glib {
protected:
    template<typename rec_t>
    inline rec_t read_record(std::istream& input);
    inline glib_archive_header read_archive_header(std::istream& input);
    inline glib_file_header read_file_header(std::istream& input);
    inline std::vector<glib_file> read_files(std::istream& input, bool bare);

    [[nodiscard]] constexpr std::istream::off_type calculate_data_offset(
        const glib_archive_header& header
    ) const noexcept;
public:
    std::vector<glib_file> list(std::istream& input);
    std::vector<glib_file> extract(std::istream& input);
};

template<typename rec_t>
rec_t glib::read_record(std::istream& input) {
    static_assert(sizeof(rec_t) == EXPECTED_HEADER_SIZE);

    if (input.bad()) {
        throw bad_stream_exception();
    }
    rec_t record;
    input.read(reinterpret_cast<char *> (&record), sizeof(rec_t));
    if (input.bad()) {
        if (input.eof()) {
            throw end_of_stream_exception();
        }
        throw bad_stream_exception();
    }
    return record;
}

#endif
