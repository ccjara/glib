#ifndef GLIB_GLIB_FILE_HXX
#define GLIB_GLIB_FILE_HXX

#include <string>
#include <vector>

class glib_file {
private:
    std::string label;
    std::vector<char> data;
public:
    glib_file(std::string&& label, std::vector<char>&& data);
    explicit glib_file(std::string&& label);
    glib_file();

    void rename(std::string&& l) noexcept;
    void rename(const char *l);

    void set_data(std::vector<char>&& d);

    [[nodiscard]] std::string get_label() const noexcept;
    [[nodiscard]] std::vector<char> get_data() const noexcept;

    [[nodiscard]] bool has_data() const noexcept;
    [[nodiscard]] bool has_label() const noexcept;
};

#endif
