#include "arg_provider.hxx"
#include "frontend.hxx"

int main(int argc, char *argv[]) {
    const auto args { arg_provider::from_main(argc, argv) };
    frontend f {};

    return f.handle(args);
}
