//
// Created by F-Mir on 4/9/2024.
//

#if __GNUC__ != 13 || __GNUC_MINOR__ != 2
#define STEPIK_CHECK_SYSTEM
#endif

#ifndef STEPIK_CHECK_SYSTEM

#include "../TestRunner/TestRunner.h"
#include "../Profiler/profile.h"

#else
#include "test_runner.h"
#include "profile.h"
#endif

#include <list>
#include <iostream>
#include <random>
#include <sstream>
#include <set>
#include <algorithm>
#include <vector>
#include <map>

template<class T>
std::ostream& operator<<(std::ostream& os, const std::list<T>& s) {
    for (const auto& x: s) {
        os << x << ' ';
    }
    return os;
}

class Sportsmen {
public:
    void GetToLine(const unsigned who, const unsigned beforeWho) {
        const auto beforeWhoIt = sportsmenMap.find(beforeWho);
        std::list<unsigned>::const_iterator newSportsmanIt;
        if (beforeWhoIt != sportsmenMap.cend())
        {
            newSportsmanIt = sportsmen.insert(beforeWhoIt->second, who);
        }
        else
        {
            newSportsmanIt = sportsmen.insert(sportsmen.cend(), who);
        }
        sportsmenMap.insert({who, newSportsmanIt});
    }

    [[nodiscard]] std::list<unsigned> GetLine() const {
        return sportsmen;
    }

private:
    std::list<unsigned> sportsmen;
    std::map<unsigned, std::list<unsigned>::const_iterator> sportsmenMap;
};

void TestWork() {
    Sportsmen sportsmen;
    sportsmen.GetToLine(42, 0);
    sportsmen.GetToLine(17, 42);
    sportsmen.GetToLine(13, 0);
    sportsmen.GetToLine(123, 42);
    sportsmen.GetToLine(5, 13);
    const std::list<unsigned> result = sportsmen.GetLine();
    ASSERT_EQUAL(std::vector(result.cbegin(), result.cend()), std::vector({17u, 123u, 42u, 5u, 13u}))
}

void TestPerformance() {
    constexpr unsigned sportsmenCount = 100000;
    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_int_distribution<int> uniform_dist(0, sportsmenCount);
    std::set<unsigned> uniqueSportsmen;
    std::vector<std::pair<unsigned, unsigned>> sportsmen;
    sportsmen.reserve(sportsmenCount);
    for (unsigned i = 0; i < sportsmenCount; i++) {
        const auto& result = uniqueSportsmen.insert(uniform_dist(e1));
        if (result.second) {
            sportsmen.emplace_back(*result.first, uniform_dist(e1));
        }
    }
    {
        LOG_DURATION("Test performance")
        Sportsmen sportsmenClass;
        for (const auto& [who, beforeWho]: sportsmen) {
            sportsmenClass.GetToLine(who, beforeWho);
        }
        std::ostringstream os;
        os << sportsmenClass.GetLine();
    }
}

void TestAll() {
    TestRunner tr;
    RUN_TEST(tr, TestWork);
    RUN_TEST(tr, TestPerformance);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    TestAll();
    unsigned count;
    std::cin >> count;
    Sportsmen sportsmen;
    for (unsigned i = 0; i < count; ++i) {
        unsigned who;
        std::cin >> who;
        unsigned beforeWho;
        std::cin >> beforeWho;
        sportsmen.GetToLine(who, beforeWho);
    }
    std::cout << sportsmen.GetLine();
    return 0;
}