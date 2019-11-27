#ifndef GLIB_GLIB_HXX
#define GLIB_GLIB_HXX

#include <istream>
#include <optional>
#include <vector>

#include "glib_file.hxx"
#include "exceptions.hxx"

constexpr auto expected_record_size { 28 };

struct glib_file_header {
    uint32_t offset; // absolute byte offset of the raw data
    char label[22]; // zero padded label
    uint16_t size; // size in bytes

    glib_file_header() : offset(0), label(""), size(0) {}
};

// glib_archive_header is a re-purposed (abstract) glib_file_header with a
// 'GLIB_FILE' label and of constant zero size which we are not going to use
struct glib_archive_header {
    uint32_t record_count;
    glib_archive_header() : record_count(0), unused("") {}
private:
    char unused[sizeof(glib_file_header::label) + sizeof(glib_file_header::size)];
};

static_assert(sizeof(glib_archive_header) == sizeof(glib_file_header));
static_assert(sizeof(glib_file_header) == expected_record_size);

class glib {
private:
    template<class rec_t>
    inline rec_t read_record(std::istream& input);
protected:
    inline glib_archive_header read_archive_header(std::istream& input);
    inline glib_file_header read_file_header(std::istream& input);

    [[nodiscard]] inline std::istream::off_type calculate_data_offset(
        const glib_archive_header& header
    ) const noexcept;
public:
    std::vector<glib_file> list(std::istream& input);
};

template<class rec_t>
rec_t glib::read_record(std::istream& input) {
    static_assert(sizeof(rec_t) == expected_record_size);

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
