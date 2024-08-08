#if __GNUC__ < 13
#define STEPIK_CHECK_SYSTEM
#endif

#ifndef STEPIK_CHECK_SYSTEM

#include "../TestRunner/TestRunner.h"

#else
#include "test_runner.h"
#endif

#include <vector>
#include <future>
#include <numeric>

using namespace std;

template<typename Iterator>
class Paginator {
    class Page {
    public:
        Page(Iterator begin, Iterator end)
                : _begin(begin), _end(end) {}

        [[nodiscard]] Iterator begin() const {
            return _begin;
        }

        [[nodiscard]] Iterator end() const {
            return _end;
        }

        [[nodiscard]] size_t size() const {
            return _end - _begin;
        }

    private:
        const Iterator _begin;
        const Iterator _end;
    };

public:
    Paginator(Iterator begin, Iterator end, size_t page_size)
            : _pages([](Iterator begin, Iterator end, size_t page_size) {
        vector<Page> pages;
        size_t difference_range = end - begin;
        while (difference_range) {
            const auto current_page_size = std::min(difference_range, page_size);
            pages.emplace_back(begin, begin + current_page_size);
            std::advance(begin, current_page_size);
            difference_range -= current_page_size;
        }
        return pages;
    }(begin, end, page_size)) {
    }

    [[nodiscard]] auto begin() const {
        return _pages.begin();
    }

    [[nodiscard]] auto end() const {
        return _pages.end();
    }

    [[nodiscard]] size_t size() const {
        return _pages.size();
    }

private:
    const vector<Page> _pages;
};

template<typename C>
auto Paginate(C &c, size_t page_size) {
    return Paginator(c.begin(), c.end(), page_size);
}

int64_t CalculateMatrixSum(const vector<vector<int>>& matrix) {
    size_t hardwareConcurrency = thread::hardware_concurrency();
    size_t threadNumber = hardwareConcurrency > 1 ? hardwareConcurrency : 2;
    int64_t result{};
    vector<future<int64_t>> futures;

    const auto sumFunction = [](const auto& page)
    {
        int64_t result{};
        for (const auto& row : page)
        {
            result = accumulate(row.cbegin(), row.cend(), result);
        }
        return result;
    };

    for (auto page : Paginate(matrix, threadNumber))
    {
        futures.emplace_back(async(sumFunction, std::move(page)));
    }

    for (auto& future : futures)
    {
        result += future.get();
    }
    return result;
}

void TestCalculateMatrixSum() {
    const vector<vector<int>> matrix = {
            {1, 2, 3, 4},
            {5, 6, 7, 8},
            {9, 10, 11, 12},
            {13, 14, 15, 16}
    };
    ASSERT_EQUAL(CalculateMatrixSum(matrix), 136);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestCalculateMatrixSum);
}
