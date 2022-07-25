#pragma once
#include <stdio.h>

#define LOG_IF(__condition, __format, ...) do {                         \
        if constexpr (__condition)                                      \
            fprintf(stderr, "[%.8s] %16s: " __format "\n",              \
                    &__TIMESTAMP__[11], __FILE_NAME__, ##__VA_ARGS__);  \
    } while(0)

#define LOG(__format, ...) \
    LOG_IF(true, __format, ##__VA_ARGS__)
