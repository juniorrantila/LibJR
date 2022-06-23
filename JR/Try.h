#pragma once

#define TRY(expression)                                \
    ({                                                 \
        auto _temporary_result = (expression);         \
        if (_temporary_result.is_error()) [[unlikely]] \
            return _temporary_result.release_error();  \
        _temporary_result.release_value();             \
    })

namespace Internal {
extern "C" int forward_fflush(void*) asm("fflush");
}

#define MUST(expression)                                  \
    ({                                                    \
        auto _result = (expression);                      \
        if (_result.is_error()) [[unlikely]] {            \
            let error = _result.release_error();          \
            let function_name = error.function_name();    \
            let message = error.message();                \
            let file = error.filename();                  \
            let line = error.line_in_file();              \
            printf("Error(MUST): %.*s: %.*s [%.*s:%d] @ " \
                   "%s [%s:%d]\n",                        \
                function_name.size(),                     \
                function_name.data(),                     \
                message.size(), message.data(),           \
                file.size(), file.data(),                 \
                line, __FUNCTION__, __FILE__, __LINE__);  \
            Internal::forward_fflush(nullptr);            \
            __builtin_abort();                            \
        }                                                 \
        _result.release_value();                          \
    })
