#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

class ReadingManager {
    using UserId = unsigned;
    using PageNumber = unsigned short;
    static constexpr UserId max_user_count = 100000;
    static constexpr PageNumber max_page_count = 1000;
public:
    ReadingManager()
            : _cumulative_sum_user_per_page([] {
        auto result_vector = vector<UserId>(max_page_count + 1);
        result_vector.at(0) = max_user_count;
        return result_vector;
    }()), _pages_per_user(vector<PageNumber>(max_user_count + 1)) {}

    void Read(int user_id, int page_count) {
        if (isNewUser(user_id)) {
            --_cumulative_sum_user_per_page[0];
            for (size_t i = page_count; i < _cumulative_sum_user_per_page.size(); ++i) {
                ++_cumulative_sum_user_per_page[i];
            }
        } else {
            const auto old_page_count = _pages_per_user[user_id];
            for (size_t i = old_page_count; i < static_cast<size_t>(page_count); ++i) {
                --_cumulative_sum_user_per_page[i];
            }
        }
        _pages_per_user[user_id] = page_count;
    }

    [[nodiscard]] double Cheer(int user_id) const {
        if (isNewUser(user_id) or _pages_per_user[user_id] == 1) {
            return 0.;
        } else if (userCount() == 1) {
            return 1.;
        }
        return _cumulative_sum_user_per_page[_pages_per_user[user_id] - 1] / static_cast<double>(userCount() - 1);
    }

private:
    [[nodiscard]] UserId userCount() const {
        return max_user_count - _cumulative_sum_user_per_page[0];
    }

    [[nodiscard]] bool isNewUser(const int userId) const {
        return _pages_per_user[userId] == 0;
    }

    vector<UserId> _cumulative_sum_user_per_page;
    vector<PageNumber> _pages_per_user;
};


int main() {
    // Для ускорения чтения данных отключается синхронизация
    // cin и cout с stdio,
    // а также выполняется отвязка cin от cout
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ReadingManager manager;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;
        int user_id;
        cin >> user_id;

        if (query_type == "READ") {
            int page_count;
            cin >> page_count;
            manager.Read(user_id, page_count);
        } else if (query_type == "CHEER") {
            cout << setprecision(6) << manager.Cheer(user_id) << "\n";
        }
    }

    return 0;
}