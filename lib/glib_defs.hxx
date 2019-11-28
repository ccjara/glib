#ifndef GLIB_GLIB_DEFS_HXX
#define GLIB_GLIB_DEFS_HXX

// a header reserves 22 bytes for the label
constexpr auto EXPECTED_LABEL_SIZE { 22U };
// each archive header struct has the same byte size
constexpr auto EXPECTED_HEADER_SIZE { 28U };
// max size of record data. theoretical limit: 2^16-1, but I have not seen
// a file larger than 64kb
constexpr auto MAX_RECORD_SIZE { 64000U };

struct glib_file_header {
    uint32_t offset; // absolute byte offset of the raw data
    char label[EXPECTED_LABEL_SIZE]; // zero padded label
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
static_assert(sizeof(glib_file_header) == EXPECTED_HEADER_SIZE);

#endif
