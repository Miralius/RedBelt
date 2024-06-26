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

#include <algorithm>
#include <array>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

// TAirport should be enum with sequential items and last item TAirport::Last_
template<typename TAirport>
class AirportCounter {
    static constexpr size_t number = static_cast<size_t>(TAirport::Last_);
public:
    // конструктор по умолчанию: список элементов пока пуст
    constexpr AirportCounter() = default;

    // конструктор от диапазона элементов типа TAirport
    template<typename TIterator>
    constexpr AirportCounter(TIterator begin, TIterator end) {
        while (begin != end) {
            ++_data[static_cast<size_t>(*begin++)];
        }
    }

    // получить количество элементов, равных данному
    [[nodiscard]] constexpr size_t Get(TAirport airport) const {
        return _data[static_cast<size_t>(airport)];
    }

    // добавить данный элемент
    constexpr void Insert(TAirport airport) {
        ++_data[static_cast<size_t>(airport)];
    }

    // удалить одно вхождение данного элемента
    constexpr void EraseOne(TAirport airport) {
        --_data[static_cast<size_t>(airport)];
    }

    // удалить все вхождения данного элемента
    constexpr void EraseAll(TAirport airport) {
        _data[static_cast<size_t>(airport)] = 0;
    }

    using Item = pair<TAirport, size_t>;
    using Items = array<Item, number>;

    // получить некоторый объект, по которому можно проитерироваться,
    // получив набор объектов типа Item - пар (аэропорт, количество),
    // упорядоченных по аэропорту
    [[nodiscard]] constexpr Items GetItems() const {
        Items result{};
        for (size_t i = 0; i < number; ++i) {
            result[i] = {static_cast<TAirport>(i), _data[i]};
        }
        return result;
    }

private:
    array<size_t, number> _data{};
};

void TestMoscow() {
    enum class MoscowAirport {
        VKO,
        SVO,
        DME,
        ZIA,
        Last_
    };

    const vector<MoscowAirport> airports = {
            MoscowAirport::SVO,
            MoscowAirport::VKO,
            MoscowAirport::ZIA,
            MoscowAirport::SVO,
    };
    AirportCounter<MoscowAirport> airport_counter(begin(airports), end(airports));

    ASSERT_EQUAL(airport_counter.Get(MoscowAirport::VKO), 1u)
    ASSERT_EQUAL(airport_counter.Get(MoscowAirport::SVO), 2u)
    ASSERT_EQUAL(airport_counter.Get(MoscowAirport::DME), 0u)
    ASSERT_EQUAL(airport_counter.Get(MoscowAirport::ZIA), 1u)

    using Item = AirportCounter<MoscowAirport>::Item;
    vector<Item> items;
    for (const auto& item: airport_counter.GetItems()) {
        items.push_back(item);
    }
    ASSERT_EQUAL(items.size(), 4u)

#define ASSERT_EQUAL_ITEM(idx, expected_enum, expected_count) \
  do { \
    ASSERT_EQUAL(static_cast<size_t>(items[idx].first), static_cast<size_t>(MoscowAirport::expected_enum)); \
    ASSERT_EQUAL(items[idx].second, expected_count); \
  } while (false)

    ASSERT_EQUAL_ITEM(0, VKO, 1u);
    ASSERT_EQUAL_ITEM(1, SVO, 2u);
    ASSERT_EQUAL_ITEM(2, DME, 0u);
    ASSERT_EQUAL_ITEM(3, ZIA, 1u);

    airport_counter.Insert(MoscowAirport::VKO);
    ASSERT_EQUAL(airport_counter.Get(MoscowAirport::VKO), 2u)

    airport_counter.EraseOne(MoscowAirport::SVO);
    ASSERT_EQUAL(airport_counter.Get(MoscowAirport::SVO), 1u)

    airport_counter.EraseAll(MoscowAirport::VKO);
    ASSERT_EQUAL(airport_counter.Get(MoscowAirport::VKO), 0u)
}

enum class SmallCountryAirports {
    Airport_1,
    Airport_2 [[maybe_unused]],
    Airport_3 [[maybe_unused]],
    Airport_4 [[maybe_unused]],
    Airport_5 [[maybe_unused]],
    Airport_6 [[maybe_unused]],
    Airport_7 [[maybe_unused]],
    Airport_8 [[maybe_unused]],
    Airport_9 [[maybe_unused]],
    Airport_10 [[maybe_unused]],
    Airport_11 [[maybe_unused]],
    Airport_12 [[maybe_unused]],
    Airport_13 [[maybe_unused]],
    Airport_14 [[maybe_unused]],
    Airport_15 [[maybe_unused]],
    Last_
};

void TestManyConstructions() {
    default_random_engine rnd(20180623); // NOLINT(*-msc51-cpp)
    uniform_int_distribution<size_t> gen_airport(
            0, static_cast<size_t>(SmallCountryAirports::Last_) - 1
    );

    array<SmallCountryAirports, 2> airports{};
    for (auto& x: airports) {
        x = static_cast<SmallCountryAirports>(gen_airport(rnd));
    }

    uint64_t total = 0;
    for (int step = 0; step < 100'000'000; ++step) {
        AirportCounter<SmallCountryAirports> counter(begin(airports), end(airports));
        total += counter.Get(SmallCountryAirports::Airport_1);
    }
    // Assert to use variable total so that compiler doesn't optimize it out
    ASSERT(total < 1000)
}

enum class SmallTownAirports {
    Airport_1,
    Airport_2 [[maybe_unused]],
    Last_
};

void TestManyGetItems() {
    default_random_engine rnd(20180701); // NOLINT(*-msc51-cpp)
    uniform_int_distribution<size_t> gen_airport(
            0, static_cast<size_t>(SmallTownAirports::Last_) - 1
    );

    array<SmallTownAirports, 2> airports{};
    for (auto& x: airports) {
        x = static_cast<SmallTownAirports>(gen_airport(rnd));
    }

    uint64_t total = 0;
    for (int step = 0; step < 100'000'000; ++step) {
        AirportCounter<SmallTownAirports> counter(begin(airports), end(airports));
        total += counter.Get(SmallTownAirports::Airport_1);
        for (const auto& [airport, count]: counter.GetItems()) {
            total += count;
        }
    }
    // Assert to use variable total so that compiler doesn't optimize it out
    ASSERT(total > 0)
}

void TestMostPopularAirport() {
    default_random_engine rnd(20180624); // NOLINT(*-msc51-cpp)
    uniform_int_distribution<size_t> gen_airport(
            0, static_cast<size_t>(SmallCountryAirports::Last_) - 1
    );

    array<pair<SmallCountryAirports, SmallCountryAirports>, 1000> daily_flight_report;
    for (auto& x: daily_flight_report) {
        x = {
                static_cast<SmallCountryAirports>(gen_airport(rnd)),
                static_cast<SmallCountryAirports>(gen_airport(rnd))
        };
    }

    const int days_to_explore = 365 * 500;

    vector<SmallCountryAirports> most_popular(days_to_explore);

    for (int day = 0; day < days_to_explore; ++day) {
        AirportCounter<SmallCountryAirports> counter;
        for (const auto& [source, dest]: daily_flight_report) {
            counter.Insert(source);
            counter.Insert(dest);
        }

        const auto items = counter.GetItems();
        most_popular[day] = max_element(begin(items), end(items), [](auto lhs, auto rhs) {
            return lhs.second < rhs.second;
        })->first;
    }

    ASSERT(all_of(begin(most_popular), end(most_popular), [&](SmallCountryAirports a) {
        return a == most_popular.front();
    }))
}

int main() {
    TestRunner tr;

    // По условию, суммарное время работы всех тестов не должно превышать
    // двух секунд. Если ваше время будет лишь чуть больше двух секунд,
    // попробуйте отправить ваше решение в тестирующую систему. Возможно,
    // там более мощное железо, и ваше решение будет принято.

    // Кроме того, не забудьте включить оптимизации при компиляции кода.

    LOG_DURATION("Total tests duration")
    RUN_TEST(tr, TestMoscow);
    RUN_TEST(tr, TestManyConstructions);
    RUN_TEST(tr, TestManyGetItems);
    RUN_TEST(tr, TestMostPopularAirport);
    return 0;
}
