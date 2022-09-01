#include <Main/Main.h>
#include <stdio.h>

static c_string g_program_name = nullptr;
c_string Main::program_name() { return g_program_name; }

int main(int argc, c_string argv[])
{
    g_program_name = argv[0];
    auto result = Main::main(argc, argv);
    if (result.is_error()) {
        auto error = result.release_error();
        auto function_name = error.function_name();
        auto message = error.message();
        auto filename = error.filename();
        fprintf(stderr, "Error: %*s: %*s [%*s:%d]\n",
            function_name.size(), function_name.data(),
            message.size(), message.data(),
            filename.size(), filename.data(),
            error.line_in_file());
        return -1;
    }
    return 0;
}
