#if __GNUC__ != 13 || __GNUC_MINOR__ != 2
#define STEPIK_CHECK_SYSTEM
#endif

#ifndef STEPIK_CHECK_SYSTEM

#include "../TestRunner/TestRunner.h"

#else
#include "test_runner.h"
#endif

#include <cstdint>
#include <numeric>
#include <vector>
#include <list>

using namespace std;

template<typename RandomIt, typename BiDirIt>
BiDirIt killVictim(list<typename RandomIt::value_type>& victims, BiDirIt selectedVictim, RandomIt killedVictims)
{
    *killedVictims = std::move(*selectedVictim);
    return victims.erase(selectedVictim);
}

template<typename BiDirIt, typename List>
BiDirIt selectNextVictim(List& victims, BiDirIt victimIt, uint32_t stepSize)
{
    bool isFirstIteration = true;
    do
    {
        if (!isFirstIteration) ++victimIt;
        if (victimIt == victims.end())
        {
            victimIt = victims.begin();
        }
        --stepSize;
        if (isFirstIteration) isFirstIteration = false;
    } while (stepSize != 0);
    return victimIt;
}

template<typename RandomIt>
void MakeJosephusPermutation(RandomIt first, RandomIt last, uint32_t step_size)
{
    list<typename RandomIt::value_type> victims;
    move(first, last, back_inserter(victims));
    auto victimIt = victims.begin();
    while (!victims.empty())
    {
        victimIt = killVictim(victims, victimIt, first++);
        victimIt = selectNextVictim(victims, victimIt, step_size);
    }
}

vector<int> MakeTestVector()
{
    vector<int> numbers(10);
    iota(begin(numbers), end(numbers), 0);
    return numbers;
}

void TestIntVector()
{
    const vector<int> numbers = MakeTestVector();
    {
        vector<int> numbers_copy = numbers;
        MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 1);
        ASSERT_EQUAL(numbers_copy, numbers)
    }
    {
        vector<int> numbers_copy = numbers;
        MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 3);
        ASSERT_EQUAL(numbers_copy, vector<int>({0, 3, 6, 9, 4, 8, 5, 2, 7, 1}))
    }
}

// Это специальный тип, который поможет вам убедиться, что ваша реализация
// функции MakeJosephusPermutation не выполняет копирование объектов.
// Сейчас вы, возможно, не понимаете как он устроен, однако мы расскажем,
// почему он устроен именно так, далее в блоке про move-семантику —
// в видео «Некопируемые типы»

struct NoncopyableInt
{
    int value;

    NoncopyableInt(const NoncopyableInt&) = delete;

    NoncopyableInt& operator=(const NoncopyableInt&) = delete;

    NoncopyableInt(NoncopyableInt&&) = default;

    NoncopyableInt& operator=(NoncopyableInt&&) = default;
};

bool operator==(const NoncopyableInt& lhs, const NoncopyableInt& rhs)
{
    return lhs.value == rhs.value;
}

ostream& operator<<(ostream& os, const NoncopyableInt& v)
{
    return os << v.value;
}

void TestAvoidsCopying()
{
    vector<NoncopyableInt> numbers;
    numbers.push_back({1});
    numbers.push_back({2});
    numbers.push_back({3});
    numbers.push_back({4});
    numbers.push_back({5});

    MakeJosephusPermutation(begin(numbers), end(numbers), 2);

    vector<NoncopyableInt> expected;
    expected.push_back({1});
    expected.push_back({3});
    expected.push_back({5});
    expected.push_back({4});
    expected.push_back({2});

    ASSERT_EQUAL(numbers, expected)
}

int main()
{
    TestRunner tr;
    RUN_TEST(tr, TestIntVector);
    RUN_TEST(tr, TestAvoidsCopying);
    return 0;
}
