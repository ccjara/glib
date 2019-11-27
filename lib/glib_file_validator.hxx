#ifndef GLIB_GLIB_FILE_VALIDATOR_HXX
#define GLIB_GLIB_FILE_VALIDATOR_HXX

#include "glib_file.hxx"

constexpr auto MAX_RECORD_SIZE { 64000U };
constexpr auto MAX_LABEL_LENGTH { 22U };

enum glib_file_validation_result {
    ok,
    no_data,
    no_label,
    label_too_long,
    data_too_large,
};

class glib_file_validator {
public:
    [[nodiscard]] glib_file_validation_result validate(
        const glib_file& file
    ) const;
};


#endif
