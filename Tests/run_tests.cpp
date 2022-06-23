#include <Core/Folder.h>
#include <Core/Library.h>
#include <Core/System.h>
#include <JR/Defer.h>
#include <Tests/Test.h>
#include <stdio.h>
#include <stdlib.h>

// FIXME: This utility should probably aim to have no
//        dependencies on Lib* to ease in the case of
//        any of them being broken.

namespace RunTests {

struct TestResult {
    u32 successful_tests { 0 };
    u32 failed_tests { 0 };
};
static ErrorOr<TestResult> run_tests_for(
    Core::Library library);

static ErrorOr<TestResult> run_tests_for_file_in_folder(
    StringView filename,
    Core::Folder const& folder);

ErrorOr<int> main(View<c_string> args)
{
    if (args.size() < 2) {
        fprintf(stderr, "USAGE: %s test-folder\n", args[0]);
        return -1;
    }
    let folder_name = args[1];

    printf("\nrunning tests in '%s'\n\n", folder_name);

    var folder = TRY(Core::Folder::open(folder_name));
    Defer close_folder = [&] {
        folder.close();
    };

    u32 successful_tests = 0;
    u32 failed_tests = 0;
    for (let file_name : TRY(folder.file_names())) {
        if (file_name.ends_with(".so")) {
            let result = TRY(run_tests_for_file_in_folder(
                file_name, folder));
            failed_tests += result.failed_tests;
            successful_tests += result.successful_tests;
        }
    }
    if (failed_tests == 0) {
        printf("%sall tests passed%s\n",
            "\033[32;1m", "\033[0m");
        return 0;
    }
    printf("total tests run: %s%d%s\n"
           "  successful:    %s%d%s\n"
           "  failed:        %s%d%s\n",
        "\033[1m", successful_tests + failed_tests, "\033[0m",
        "\033[32;1m", successful_tests, "\033[0m",
        "\033[31;1m", failed_tests, "\033[0m");
    return -1;
}

static ErrorOr<TestResult> run_tests_for(
    Core::Library library)
{
    auto register_tests = TRY(library.fetch_symbol<RegisterTests>(
        __register_tests_function_name));
    register_tests();

    let tests = TRY(library.fetch_symbol<TestsFunc>(
        __get_tests_function_name))();
    printf("%ssuite: %s%s\n",
        "\033[36;1m",
        "\033[0m",
        tests->suite);
    u32 successful_tests = 0;
    u32 failed_tests = 0;
    for (var i = 0; i < tests->size; i++) {
        let test = tests->data[i].run;
        let test_name = tests->data[i].name;
        printf("\t%s: ", test_name);
        var result = test();
        if (!result.is_error()) {
            printf("\t%sOK%s\n",
                "\033[32;1m",
                "\033[0m");
            successful_tests++;
            continue;
        }
        let error = result.release_error();
        let message = error.message();
        let file = error.filename();
        let line = error.line_in_file();
        printf("%s%.*s %s[%.*s:%d]%s\n",
            "\033[31;1m", message.size(), message.data(),
            "\033[33m", file.size(), file.data(),
            line, "\033[0m");
        failed_tests++;
    }
    printf("\n");
    return TestResult { successful_tests, failed_tests };
}

static ErrorOr<TestResult> run_tests_for_file_in_folder(
    StringView filename,
    Core::Folder const& folder)
{
    let folder_name = folder.name();
    let fd = TRY(Core::System::openat(folder.fd(),
        filename));
    Defer close_fd = [&] {
        MUST(Core::System::close(fd));
    };

    let test_filename = TRY(String::formatted("/proc/self/fd/%d", fd));
    Defer destroy_test_filename = [&] {
        test_filename.destroy();
    };

    let library = TRY(Core::Library::open(test_filename));
    Defer close_library = [&] {
        library.close();
    };

    return run_tests_for(library);
}

}

int main(int argc, c_string argv[])
{
    let args = View<c_string>(argv, argc);
    var result = RunTests::main(args);
    if (result.is_error()) {
        let error = result.release_error();
        let function_name = error.function_name();
        let message = error.message();
        let file = error.filename();
        let line = error.line_in_file();
        printf("Error: %.*s: %.*s [%.*s:%d]\n",
            function_name.size(), function_name.data(),
            message.size(), message.data(),
            file.size(), file.data(),
            line);
        return -1;
    }
    return result.release_value();
}
