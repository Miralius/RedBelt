#if __GNUC__ == 13 && __GNUC_MINOR__ == 2

#include "../TestRunner/TestRunner.h"

#else
#include "test_runner.h"
#endif

using namespace std;

template<class T>
class Table {
public:
    Table(const size_t rows, const size_t columns)
            : _table({rows, vector<T>(columns)}) {
    }

    decltype(auto) operator[](const size_t index) const {
        return _table[index];
    }

    decltype(auto) operator[](const size_t index) {
        return _table[index];
    }

    void Resize(const size_t rows, const size_t columns) {
        _table.resize(rows);
        for (auto &row: _table) {
            row.resize(columns);
        }
    }

    [[nodiscard]] decltype(auto) Size() const {
        return make_pair(_table.size(), _table.empty() ? 0u : _table.front().size());
    }

private:
    vector<vector<T>> _table;
};

void TestTable() {
    Table<int> t(1, 1);
    ASSERT_EQUAL(t.Size().first, 1u)
    ASSERT_EQUAL(t.Size().second, 1u)
    t[0][0] = 42;
    ASSERT_EQUAL(t[0][0], 42)
    t.Resize(3, 4);
    ASSERT_EQUAL(t.Size().first, 3u)
    ASSERT_EQUAL(t.Size().second, 4u)
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestTable);
    return 0;
}
