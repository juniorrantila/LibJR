#include <JR/String.h>
#include <JR/Types.h>
#include <stdio.h>
#include <stdlib.h>

void String::destroy() const
{
    if (is_valid()) {
        free(m_data);
        invalidate();
    }
}

ErrorOr<String> String::formatted(c_string format, ...)
{
    va_list args;
    va_start(args, format);
    char* output = nullptr;
    var size = vasprintf(&output, format, args);
    if (size == -1)
        return Error::allocation_failure();
    va_end(args);
    return String { output, (u32)size };
}
