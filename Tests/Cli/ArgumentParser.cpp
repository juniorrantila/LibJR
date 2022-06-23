#include <Tests/Test.h>
#include <Cli/ArgumentParser.h>

TEST_CASE(create_and_destroy)
{
    var parser = TRY(Cli::ArgumentParser::create());
    parser.destroy();
    EXPECT(!parser.is_valid());
    return {};
}

REGISTER_TESTS()
{
    REGISTER_TEST(create_and_destroy);
}
