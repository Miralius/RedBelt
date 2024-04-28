#if __GNUC__ != 13 || __GNUC_MINOR__ != 2
#define STEPIK_CHECK_SYSTEM
#endif

#ifndef STEPIK_CHECK_SYSTEM

#include "../TestRunner/TestRunner.h"

#else
#include "test_runner.h"
#endif

#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <map>

using namespace std;

// Объявляем Group<String> для произвольного типа String
// синонимом vector<String>.
// Благодаря этому в качестве возвращаемого значения
// функции можно указать не малопонятный вектор векторов,
// а вектор групп — vector<Group<String>>.
template<typename String>
using Group = vector<String>;

// Ещё один шаблонный синоним типа
// позволяет вместо громоздкого typename String::value_type
// использовать Char<String>
template<typename String>
using Char = typename String::value_type;

template<typename String>
vector<Group<String>> GroupHeavyStrings(vector<String> strings)
{
    map<set<Char<String>>, Group<String>> groupsMap;
    for (auto& string : strings) {
        groupsMap[{string.begin(), string.end()}].emplace_back(std::move(string));
    }
    vector<Group<String>> groups;
    for (auto& [key, group] : groupsMap) {
        groups.push_back(std::move(group));
    }
    return groups;
}


void TestGroupingABC()
{
    vector<string> strings = {"caab", "abc", "cccc", "bacc", "c"};
    auto groups = GroupHeavyStrings(strings);
    ASSERT_EQUAL(groups.size(), 2u)
    sort(begin(groups), end(groups));  // Порядок групп не имеет значения
    ASSERT_EQUAL(groups[0], vector<string>({"caab", "abc", "bacc"}))
    ASSERT_EQUAL(groups[1], vector<string>({"cccc", "c"}))
}

void TestGroupingReal()
{
    vector<string> strings = {"law", "port", "top", "laptop", "pot", "paloalto", "wall", "awl"};
    auto groups = GroupHeavyStrings(strings);
    ASSERT_EQUAL(groups.size(), 4u)
    sort(begin(groups), end(groups));  // Порядок групп не имеет значения
    ASSERT_EQUAL(groups[0], vector<string>({"laptop", "paloalto"}))
    ASSERT_EQUAL(groups[1], vector<string>({"law", "wall", "awl"}))
    ASSERT_EQUAL(groups[2], vector<string>({"port"}))
    ASSERT_EQUAL(groups[3], vector<string>({"top", "pot"}))
}

int main()
{
    TestRunner tr;
    RUN_TEST(tr, TestGroupingABC);
    RUN_TEST(tr, TestGroupingReal);
    return 0;
}
