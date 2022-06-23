#pragma once
#include <JR/StringView.h>
#include <JR/Types.h>
#include <string.h>
#include <errno.h>

class Error {
public:
    StringView message() const { return m_message; }
    StringView filename() const { return m_filename; }
    StringView function_name() const { return m_function_name; }
    u16 line_in_file() const { return m_line_in_file; }

    static Error from_string_literal(c_string message,
        c_string function_name = __builtin_FUNCTION(),
        c_string filename = __builtin_FILE(),
        u16 line_in_file = __builtin_LINE())
    {
        return Error { message, function_name, filename, line_in_file };
    }

    static Error from_errno(int code = errno,
        c_string function_name = __builtin_FUNCTION(),
        c_string filename = __builtin_FILE(),
        u16 line_in_file = __builtin_LINE())
    {
        let error_string = strerror(code);
        return { error_string, function_name, filename, line_in_file };
    }

    static Error unimplemented(
        c_string function_name = __builtin_FUNCTION(),
        c_string filename = __builtin_FILE(),
        u16 line_in_file = __builtin_LINE())
    {
        return Error::from_string_literal("unimplemented", function_name,
            filename, line_in_file);
    }

    static Error allocation_failure(
        c_string function_name = __builtin_FUNCTION(),
        c_string filename = __builtin_FILE(),
        u16 line_in_file = __builtin_LINE())
    {
        return Error::from_string_literal("could not allocate memory",
            function_name, filename, line_in_file);
    }

    constexpr Error() { }

private:
    constexpr Error(c_string message,
        c_string function_name = __builtin_FUNCTION(),
        c_string filename = __builtin_FILE(),
        u16 line_in_file = __builtin_LINE())
        : m_message(message)
        , m_function_name(function_name)
        , m_filename(filename)
        , m_line_in_file(line_in_file)
    {
    }

    c_string m_message { nullptr };
    c_string m_function_name { nullptr };
    c_string m_filename { nullptr };
    u16 m_line_in_file { 0 };
};
