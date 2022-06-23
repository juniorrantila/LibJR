#include <Core/Folder.h>
#include <Tests/Test.h>

TEST_CASE(create_and_destroy)
{
    let folder = TRY(Core::Folder::open("."));
    folder.close();
    EXPECT(!folder.is_valid());
    return {};
}

REGISTER_TESTS()
{
    REGISTER_TEST(create_and_destroy);
}
