#include <Tests/Test.h>
#include <Core/Thread.h>

TEST_CASE(create_and_join)
{
    var thread = TRY(Core::Thread::create([]{}));
    thread.start();
    thread.join();
    return {};
}

REGISTER_TESTS()
{
    REGISTER_TEST(create_and_join);
}
