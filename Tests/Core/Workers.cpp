#include <Tests/Test.h>
#include <Core/Workers.h>

TEST_CASE(create_and_destroy)
{
    let workers = TRY(Core::Workers::create_default_amount());
    workers.destroy();
    EXPECT(!workers.is_valid());
    return {};
}

REGISTER_TESTS()
{
    REGISTER_TEST(create_and_destroy);
}
