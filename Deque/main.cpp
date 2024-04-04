#if __GNUC__ == 13 && __GNUC_MINOR__ == 2

#include "../TestRunner/TestRunner.h"

#else
#include "test_runner.h"
#endif

#include "Deque.h"

using namespace std;

void TestDeque() {
    Deque<int> t;
    ASSERT(t.Empty())
    ASSERT_EQUAL(t.Size(), 0u)

    t.PushFront(8);
    ASSERT(!t.Empty())
    ASSERT_EQUAL(t.Size(), 1u)
    ASSERT_EQUAL(t[0], 8)
    ASSERT_EQUAL(t.At(0), 8)
    ASSERT_EQUAL(t.Front(), 8)
    ASSERT_EQUAL(t.Back(), 8)

    t.PushFront(7);
    ASSERT(!t.Empty())
    ASSERT_EQUAL(t.Size(), 2u)
    ASSERT_EQUAL(t[0], 7)
    ASSERT_EQUAL(t[1], 8)
    ASSERT_EQUAL(t.At(0), 7)
    ASSERT_EQUAL(t.At(1), 8)
    ASSERT_EQUAL(t.Front(), 7)
    ASSERT_EQUAL(t.Back(), 8)

    t[1] = 9;
    ASSERT_EQUAL(t[1], 9)
    ASSERT_EQUAL(t.At(1), 9)
    ASSERT_EQUAL(t.Front(), 7)
    ASSERT_EQUAL(t.Back(), 9)

    t[1] = 8;
    t.PushFront(11);
    ASSERT(!t.Empty())
    ASSERT_EQUAL(t.Size(), 3u)
    ASSERT_EQUAL(t[0], 11)
    ASSERT_EQUAL(t[1], 7)
    ASSERT_EQUAL(t[2], 8)
    ASSERT_EQUAL(t.At(0), 11)
    ASSERT_EQUAL(t.At(1), 7)
    ASSERT_EQUAL(t.At(2), 8)
    ASSERT_EQUAL(t.Front(), 11)
    ASSERT_EQUAL(t.Back(), 8)

    t.PushBack(7);
    ASSERT(!t.Empty())
    ASSERT_EQUAL(t.Size(), 4u)
    ASSERT_EQUAL(t[0], 11)
    ASSERT_EQUAL(t[1], 7)
    ASSERT_EQUAL(t[2], 8)
    ASSERT_EQUAL(t[3], 7)
    ASSERT_EQUAL(t.At(0), 11)
    ASSERT_EQUAL(t.At(1), 7)
    ASSERT_EQUAL(t.At(2), 8)
    ASSERT_EQUAL(t.At(3), 7)
    ASSERT_EQUAL(t.Front(), 11)
    ASSERT_EQUAL(t.Back(), 7)

    t.PushBack(-2);
    ASSERT(!t.Empty())
    ASSERT_EQUAL(t.Size(), 5u)
    ASSERT_EQUAL(t[0], 11)
    ASSERT_EQUAL(t[1], 7)
    ASSERT_EQUAL(t[2], 8)
    ASSERT_EQUAL(t[3], 7)
    ASSERT_EQUAL(t[4], -2)
    ASSERT_EQUAL(t.At(0), 11)
    ASSERT_EQUAL(t.At(1), 7)
    ASSERT_EQUAL(t.At(2), 8)
    ASSERT_EQUAL(t.At(3), 7)
    ASSERT_EQUAL(t.At(4), -2)
    ASSERT_EQUAL(t.Front(), 11)
    ASSERT_EQUAL(t.Back(), -2)

    t.PushBack(-3);
    t.PushBack(-4);
    ASSERT_EQUAL(t.Size(), 7u)
    ASSERT_EQUAL(t.Back(), -4)
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestDeque);
    return 0;
}