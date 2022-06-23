#include <Tests/Test.h>
#include <JR/Vector.h>
#include <JR/Defer.h>

TEST_CASE(create_and_destroy)
{
    var vec = TRY(Vector<int>::create());
    vec.destroy();
    EXPECT(!vec.is_valid());
    return {};
}

TEST_CASE(append)
{
    var vec = TRY(Vector<int>::create());
    Defer destroy_vec = [&] {
        vec.destroy();
    };
    TRY(vec.append(42));
    TRY(vec.append(11));
    TRY(vec.append(420));
    TRY(vec.append(2020));
    EXPECT(vec[0] == 42);
    EXPECT(vec[1] == 11);
    EXPECT(vec[2] == 420);
    EXPECT(vec[3] == 2020);
    return {};
}

REGISTER_TESTS() {
    REGISTER_TEST(create_and_destroy);
    REGISTER_TEST(append);
}
