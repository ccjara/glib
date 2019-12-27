#include "glib_file.hxx"

glib_file::glib_file(std::string&& label, std::vector<char>&& container) :
    label(std::move(label)),
    data_container(std::move(container))
    {
    size = data_container.size();
}

glib_file::glib_file(
    std::string&& label,
    std::vector<char>::size_type size
) :
    label(std::move(label)),
    size(size) {
}

glib_file::glib_file() :
    size(0) {
}

void glib_file::rename(std::string&& l) noexcept {
    label = std::move(l);
}

void glib_file::rename(const char* l) {
    label = l;
}

void glib_file::set_container(std::vector<char>&& container) {
    data_container = std::move(container);
    size = data_container.size();
}

const std::string& glib_file::get_label() const noexcept {
    return label;
}

const std::vector<char>& glib_file::get_container() const noexcept {
    return data_container;
}

bool glib_file::has_data() const noexcept {
    return !data_container.empty();
}

bool glib_file::has_label() const noexcept {
    return !label.empty();
}

void glib_file::set_size(const std::vector<char>::size_type s) noexcept {
    size = s;
}

std::vector<char>::size_type glib_file::get_size() const noexcept {
    return size;
}
