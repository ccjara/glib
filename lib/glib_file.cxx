#include "glib_file.hxx"

glib_file::glib_file(std::string&& label, std::vector<char>&& data) :
    label(label),
    data(data) {
}

glib_file::glib_file(std::string &&label):
    label(label) {
}

glib_file::glib_file() = default;

void glib_file::rename(std::string&& l) noexcept {
    label = l;
}

void glib_file::rename(const char *l) {
    label = l;
}

void glib_file::set_data(std::vector<char> &&d) {
    data = d;
}

std::string glib_file::get_label() const noexcept {
    return label;
}

std::vector<char> glib_file::get_data() const noexcept {
    return data;
}

bool glib_file::has_data() const noexcept {
    return !data.empty();
}

bool glib_file::has_label() const noexcept {
    return !label.empty();
}
