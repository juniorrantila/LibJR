#include <Core/MappedFile.h>
#include <Tests/Test.h>
#include <stdlib.h>

TEST_CASE(create_and_destroy)
{
    let filename = getenv("_");
    let file = TRY(Core::MappedFile::open(filename));
    file.close();
    EXPECT(!file.is_valid());
    return {};
}

REGISTER_TESTS()
{
    REGISTER_TEST(create_and_destroy);
}
