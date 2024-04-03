#include "airline_ticket.h"

#if __GNUC__ == 13 && __GNUC_MINOR__ == 2

#include "../TestRunner/TestRunner.h"

#else
#include "test_runner.h"
#endif

#include <algorithm>

using namespace std;

#define SORT_BY(field)                                      \
[](const AirlineTicket& lhs, const AirlineTicket& rhs) {    \
    return lhs.field < rhs.field;                           \
}

bool operator<(const Date &lhs, const Date &rhs) {
    return tie(lhs.year, lhs.month, lhs.day) < tie(rhs.year, rhs.month, rhs.day);
}

bool operator==(const Date &lhs, const Date &rhs) {
    return tie(lhs.year, lhs.month, lhs.day) == tie(rhs.year, rhs.month, rhs.day);
}

ostream &operator<<(ostream &os, const Date &obj) {
    return os << obj.year << '.' << obj.month << '.' << obj.day;
}

bool operator<(const Time &lhs, const Time &rhs) {
    return tie(lhs.hours, lhs.minutes) < tie(rhs.hours, rhs.minutes);
}

bool operator==(const Time &lhs, const Time &rhs) {
    return tie(lhs.hours, lhs.minutes) == tie(rhs.hours, rhs.minutes);
}

ostream &operator<<(ostream &os, const Time &obj) {
    return os << obj.hours << ':' << obj.minutes;
}

void TestSortBy() {
    vector<AirlineTicket> tickets = {
            {"VKO", "AER", "Utair",     {2018, 2, 28}, {17, 40}, {2018, 2, 28}, {20, 0},  1200},
            {"AER", "VKO", "Utair",     {2018, 3, 5},  {14, 15}, {2018, 3, 5},  {16, 30}, 1700},
            {"AER", "SVO", "Aeroflot",  {2018, 3, 5},  {18, 30}, {2018, 3, 5},  {20, 30}, 2300},
            {"PMI", "DME", "Iberia",    {2018, 2, 8},  {23, 00}, {2018, 2, 9},  {3,  30}, 9000},
            {"CDG", "SVO", "AirFrance", {2018, 3, 1},  {13, 00}, {2018, 3, 1},  {17, 30}, 8000},
    };

    sort(begin(tickets), end(tickets), SORT_BY(price));
    ASSERT_EQUAL(tickets.front().price, 1200)
    ASSERT_EQUAL(tickets.back().price, 9000)

    sort(begin(tickets), end(tickets), SORT_BY(from));
    ASSERT_EQUAL(tickets.front().from, "AER")
    ASSERT_EQUAL(tickets.back().from, "VKO")

    sort(begin(tickets), end(tickets), SORT_BY(arrival_date));
    ASSERT_EQUAL(tickets.front().arrival_date, (Date{2018, 2, 9}))
    ASSERT_EQUAL(tickets.back().arrival_date, (Date{2018, 3, 5}))
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSortBy);
}
