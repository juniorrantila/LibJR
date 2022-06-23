#include <Tests/Test.h>
#include <JR/Queue.h>
#include <JR/Defer.h>

TEST_CASE(create_and_destroy)
{
    var queue = TRY(Queue<int>::create());
    queue.destroy();
    EXPECT(!queue.is_valid());
    return {};
}

TEST_CASE(enqueue_and_dequeue)
{
    var queue = TRY(Queue<int>::create());
    Defer destroy_queue = [&] {
        queue.destroy();
    };

    EXPECT(queue.is_empty());

    TRY(queue.enqueue(42));
    TRY(queue.enqueue(11));
    TRY(queue.enqueue(420));
    TRY(queue.enqueue(2020));

    EXPECT(!queue.is_empty());

    EXPECT(queue.unchecked_dequeue() == 42);
    EXPECT(queue.unchecked_dequeue() == 11);
    EXPECT(queue.unchecked_dequeue() == 420);
    EXPECT(queue.unchecked_dequeue() == 2020);

    EXPECT(queue.is_empty());

    return {};
}

REGISTER_TESTS() {
    REGISTER_TEST(create_and_destroy);
    REGISTER_TEST(enqueue_and_dequeue);
}
