#include "arg_provider.hxx"
#include "frontend.hxx"

constexpr auto exit_success { 0U };
constexpr auto exit_failure { 1U };

int main(int argc, char *argv[]) {
    frontend f {};

    if (f.handle(arg_provider::from_main(argc, argv))) {
        return exit_success;
    }
    return exit_failure;
}
