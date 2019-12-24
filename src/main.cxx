#include "arg_provider.hxx"
#include "frontend.hxx"

int main(int argc, char *argv[]) {
    const arg_provider args { argc, argv };
    frontend f {};

    return f.handle(args);
}
