#pragma once
#include <JR/null.h>

using i8 = char;
using i16 = short;
using i32 = int;
using i64 = long long;

using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;

using f32 = float;
using f64 = double;
using f128 = long double;

using string_literal = char const* nonnull const&&;
using c_string = char const*;

using usize = u64;
using isize = i64;

#ifdef __unix__
#include <unistd.h>
#else
using intptr_t = i64;
using uintptr_t = u64;
static_assert(sizeof(intptr_t) == sizeof(void*));
static_assert(sizeof(uintptr_t) == sizeof(void*));
#endif
