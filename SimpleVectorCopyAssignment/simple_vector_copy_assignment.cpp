#include "simple_vector.h"

#if __GNUC__ != 13 || __GNUC_MINOR__ != 2
#define STEPIK_CHECK_SYSTEM
#endif

#ifndef STEPIK_CHECK_SYSTEM

#include "../TestRunner/TestRunner.h"

#else
#include "test_runner.h"
#endif

#include <numeric>
#include <vector>

using namespace std;

void TestCopyAssignment()
{
    SimpleVector<int> numbers(10);
    iota(numbers.begin(), numbers.end(), 1);

    SimpleVector<int> dest;
    ASSERT_EQUAL(dest.Size(), 0u)

    dest = numbers;
    ASSERT_EQUAL(dest.Size(), numbers.Size())
    ASSERT(dest.Capacity() >= dest.Size())
    ASSERT(equal(dest.begin(), dest.end(), numbers.begin()));
}

int main()
{
    TestRunner tr;
    RUN_TEST(tr, TestCopyAssignment);
}
