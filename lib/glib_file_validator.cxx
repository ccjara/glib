#include "glib_file_validator.hxx"

glib_file_validation_result glib_file_validator::validate(
    const glib_file &file
) {
    const auto& container { file.get_container() };
    if (container.empty()) {
        return glib_file_validation_result::no_data;
    } else if (container.size() > MAX_RECORD_SIZE) {
        return glib_file_validation_result::data_too_large;
    }
    const auto& label { file.get_label() };
    if (label.empty()) {
        return glib_file_validation_result::no_label;
    } else if (label.length() > FIXED_LABEL_SIZE) {
        return glib_file_validation_result::label_too_long;
    }
    return glib_file_validation_result::ok;
}
