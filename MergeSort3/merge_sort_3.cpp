#if __GNUC__ != 13 || __GNUC_MINOR__ != 2
#define STEPIK_CHECK_SYSTEM
#endif

#ifndef STEPIK_CHECK_SYSTEM

#include "../TestRunner/TestRunner.h"

#else
#include "test_runner.h"
#endif

#include <algorithm>
#include <vector>

using namespace std;

template<typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end)
{
    // 1. Если диапазон содержит меньше 2 элементов, выйти из функции
    if (distance(range_begin, range_end) < 2)
    {
        return;
    }
    // 2. Создать вектор, содержащий все элементы текущего диапазона
    vector<typename RandomIt::value_type> sorted(std::make_move_iterator(range_begin),
                                                 std::make_move_iterator(range_end));
    // 3. Разбить вектор на 3 равные части
    auto a_third = distance(sorted.begin(), sorted.end()) / 3;
    auto first_part_range_begin = sorted.begin();
    auto first_part_range_end = sorted.begin() + a_third;
    auto second_part_range_begin = first_part_range_end;
    auto second_part_range_end = second_part_range_begin + a_third;
    auto third_part_range_begin = second_part_range_end;
    auto third_part_range_end = sorted.end();
    // 4. Вызвать функцию MergeSort от каждой части вектора.
    MergeSort(first_part_range_begin, first_part_range_end);
    MergeSort(second_part_range_begin, second_part_range_end);
    MergeSort(third_part_range_begin, third_part_range_end);
    // 5. Слить первые две трети вектора с помощью алгоритма merge,
    // сохранив результат во временный вектор с помощью back_inserter
    vector<typename RandomIt::value_type> temp_vector;
    merge(std::make_move_iterator(first_part_range_begin), std::make_move_iterator(first_part_range_end),
          std::make_move_iterator(second_part_range_begin), std::make_move_iterator(second_part_range_end),
          back_inserter(temp_vector));
    // 6. Слить временный вектор из предыдущего пункта с последней третью вектора из п. 4,
    // записав полученный отсортированный диапазон вместо исходного.
    merge(std::make_move_iterator(temp_vector.begin()), std::make_move_iterator(temp_vector.end()),
          std::make_move_iterator(third_part_range_begin), std::make_move_iterator(third_part_range_end), range_begin);
}

struct NoncopyableInt
{
    NoncopyableInt(int i) // NOLINT(*-explicit-constructor)
    : value(i)
    {
    }

    int value;

    NoncopyableInt(const NoncopyableInt&) = delete;

    NoncopyableInt& operator=(const NoncopyableInt&) = delete;

    NoncopyableInt(NoncopyableInt&&) = default;

    NoncopyableInt& operator=(NoncopyableInt&&) = default;
};

bool operator<(const NoncopyableInt& lhs, const NoncopyableInt& rhs)
{
    return lhs.value < rhs.value;
}

void TestIntVector()
{
    vector<NoncopyableInt> numbers;
    numbers.emplace_back(6);
    numbers.emplace_back(1);
    numbers.emplace_back(3);
    numbers.emplace_back(9);
    numbers.emplace_back(1);
    numbers.emplace_back(9);
    numbers.emplace_back(8);
    numbers.emplace_back(12);
    numbers.emplace_back(1);
    MergeSort(begin(numbers), end(numbers));
    ASSERT(is_sorted(begin(numbers), end(numbers)));
}

int main()
{
    TestRunner tr;
    RUN_TEST(tr, TestIntVector);
    return 0;
}
