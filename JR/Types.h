#pragma once
#include <JR/null.h>

using i8 = char;
using i16 = short;
using i32 = int;
using i64 = long;
static_assert(sizeof(i8) == 1);
static_assert(sizeof(i16) == 2);
static_assert(sizeof(i32) == 4);
static_assert(sizeof(i64) == 8);

using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long;
static_assert(sizeof(u8) == 1);
static_assert(sizeof(u16) == 2);
static_assert(sizeof(u32) == 4);
static_assert(sizeof(u64) == 8);

using f32 = float;
using f64 = double;
using f128 = long double;
static_assert(sizeof(f32) == 4);
static_assert(sizeof(f64) == 8);
static_assert(sizeof(f128) == 16);

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
