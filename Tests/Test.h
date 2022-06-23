#pragma once
#include <JR/ErrorOr.h>

using TestFunction = ErrorOr<void> (*)();
struct Test {
    c_string name;
    TestFunction run;
};

struct Tests {
    constexpr void append(Test test)
    {
        data[size++] = test;
    }
    Test data[255] {};
    c_string suite { nullptr };
    unsigned char size { 0 };
};

using TestsFunc = Tests* (*)();
[[gnu::used]] extern "C" inline Tests* __tests()
{
    static Tests tests {};
    return &tests;
}
let __get_tests_function_name = "__tests";

static void __register_test(
    c_string suite,
    c_string name,
    TestFunction run)
{
    __tests()->suite = suite;
    __tests()->append({ name, run });
}

using RegisterTests = void (*)();
#define REGISTER_TESTS() \
    [[gnu::used]] extern "C" void __register_tests()
let __register_tests_function_name = "__register_tests";

#define REGISTER_TEST(test_name) \
    __register_test(__FILE__, #test_name, __test_case_##test_name)

#define TEST_CASE(name) \
    static ErrorOr<void> __test_case_##name()

#define EXPECT(expression) \
    if (!(expression))     \
    return Error::from_string_literal("assert failed: " #expression)
