#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <set>

using namespace std;

class RouteManager {
public:
    void AddRoute(int start, int finish) {
        reachable_lists_[start].insert(finish);
        reachable_lists_[finish].insert(start);
    }

    [[nodiscard]] int FindNearestFinish(int start, int finish) const {
        int result = abs(start - finish);
        const auto reachable_stations_it = reachable_lists_.find(start);
        if (reachable_stations_it == reachable_lists_.cend()) {
            return result;
        }
        const auto& reachable_stations = reachable_stations_it->second;
        const auto finish_pos = reachable_stations.lower_bound(finish);
        if (finish_pos != end(reachable_stations)) {
            result = min(result, abs(finish - *finish_pos));
        }
        if (finish_pos != begin(reachable_stations)) {
            result = min(result, abs(finish - *prev(finish_pos)));
        }
        return result;
    }

private:
    map<int, set<int>> reachable_lists_;
};


int main() {
    RouteManager routes;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;
        int start, finish;
        cin >> start >> finish;
        if (query_type == "ADD") {
            routes.AddRoute(start, finish);
        } else if (query_type == "GO") {
            cout << routes.FindNearestFinish(start, finish) << "\n";
        }
    }

    return 0;
}
