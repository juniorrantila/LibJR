#pragma once

#ifdef __clang__
#    define nonnull _Nonnull
#    define nullable _Nullable
#else
#    define nonnull
#    define nullable
#endif

