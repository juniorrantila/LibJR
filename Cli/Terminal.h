#pragma once
#include <JR/Types.h>
#include <stdio.h>

namespace Cli {

class Terminal {
public:
    static constexpr void hide_cursor(FILE* file = stdout)
    {
        fprintf(file, "\033[?25l");
        fflush(file);
    }

    static constexpr void show_cursor(FILE* file = stdout)
    {
        fprintf(file, "\033[?25h");
        fflush(file);
    }

    static constexpr void move_cursor_to_start_of_line(
        u32 line, FILE* file = stdout)
    {
        fprintf(file, "\033[%d;0H", line);
        fflush(file);
    }

    static constexpr void clear_current_line(
        FILE* file = stdout)
    {
        fprintf(file, "\033[2K");
        fflush(file);
    }

    static constexpr void save_screen(FILE* file = stdout)
    {
        fprintf(file, "\033[?47h");
        fflush(file);
    }

    static constexpr void clear_screen(FILE* file = stdout)
    {
        fprintf(file, "\33[2J");
        fflush(file);
    }

    static constexpr void restore_screen(
        FILE* file = stdout)
    {
        fprintf(file, "\033[?47l");
        fflush(file);
    }
};

}
