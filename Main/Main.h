#pragma once
#include <JR/ErrorOr.h>
#include <JR/Types.h>

struct Main {
    static c_string program_name();
    static ErrorOr<int> main(int argc, c_string argv[]);
};
