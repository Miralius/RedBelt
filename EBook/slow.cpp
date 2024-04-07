#include <iomanip>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <numeric>

using namespace std;

class ReadingManager {
public:
    ReadingManager()
            : _users_per_page(vector<set<UserId>>(1000)) {}

    void Read(int user_id, int page_count) {
        --page_count; // use page_count as index
        auto user_it = _pages_per_user.find(user_id);
        if (user_it == _pages_per_user.cend())
        {
            _pages_per_user.insert({ user_id, page_count }); // O(log (all_users))
            _users_per_page[page_count].insert(user_id); // O(log (all_users))
        }
        else
        {
            auto& page_count_ref = user_it->second;
            _users_per_page[page_count_ref].erase(user_id); // O(log (all_pages))
            page_count_ref = page_count;
            _users_per_page[page_count].insert(user_id); // O(log (all_pages))
        }
    }

    [[nodiscard]] double Cheer(int user_id) const {
        const auto user_it = _pages_per_user.find(user_id);
        if (user_it == _pages_per_user.cend())
        {
            return 0.;
        }
        else
        {
            const auto end = _users_per_page.cbegin() + user_it->second;
            const auto count = accumulate(_users_per_page.cbegin(), end, 0u, [](const auto lhs, const auto rhs)
            {
                return lhs + rhs.size();
            });
            const auto all_users_without_current = _pages_per_user.size() - 1;
            return all_users_without_current ? count / static_cast<double>(all_users_without_current) : 1.;
        }
    }

private:
    using UserId = unsigned;
    using PageNumber = unsigned short;
    vector<set<UserId>> _users_per_page;
    map<UserId, PageNumber> _pages_per_user;
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