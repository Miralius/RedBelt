//
// Created by F-Mir on 4/9/2024.
//
// The #if is needed for stepik check system submitting if we use the whole file
#if __GNUC__ == 13 && __GNUC_MINOR__ == 2

#include "../TestRunner/TestRunner.h"
#include "../Profiler/profile.h"

#else
#include "test_runner.h"
#include "profile.h"
#endif

#include <list>

class Sportsmen {
public:

private:

};

void TestWork()
{
    constexpr unsigned count = 5;
}

void TestPerformance()
{

}

void TestAll()
{
    TestRunner tr;
    RUN_TEST(tr, TestWork);
    RUN_TEST(tr, TestPerformance);
}

int main()
{
    TestAll();
    return 0;
}