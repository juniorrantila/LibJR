#include <Cli/ProgressBar.h>
#include <JR/Defer.h>
#include <Tests/Test.h>
#include <cstdio>
#include <unistd.h>

TEST_CASE(create_and_destroy)
{
    var progress_bars = TRY(Cli::ProgressBars::create());
    progress_bars.destroy();
    EXPECT(!progress_bars.is_valid());
    return {};
}

TEST_CASE(create_single_bar)
{
    var progress_bars = TRY(Cli::ProgressBars::create());
    Defer destroy_progress_bars = [&] {
        progress_bars.destroy();
    };
    var bar = TRY(progress_bars.create_bar("foo"));
    return {};
}

REGISTER_TESTS()
{
    REGISTER_TEST(create_and_destroy);
    REGISTER_TEST(create_single_bar);
}
