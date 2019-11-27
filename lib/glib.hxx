#ifndef GLIBEX_GLIB_HXX
#define GLIBEX_GLIB_HXX

#include "exceptions.hxx"

constexpr uint16_t MAX_RECORD_SIZE { 64000U };

enum class error_code {
    bad_input,
    eof,
};

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

glib_header read_header(std::istream& input) {
    if (input.bad()) {
        throw bad_stream_exception();
    }
    glib_header header;
    input.read(reinterpret_cast<char *> (&header), sizeof(header));
    if (input.bad()) {
        if (input.eof()) {
            throw end_of_stream_exception();
        }
        throw bad_stream_exception();
    }
    return header;
}

#endif
