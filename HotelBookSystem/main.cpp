//
// Created by F-Mir on 4/7/2024.
//
#include <iostream>

using namespace std;

//time            int64_t
//hotel_name      string
//client_id       uint32_t
//room_count      uint16_t
//SUM(client_id)  uint32_t
//SUM(room_count) uint64_t
//
//ROOMS   hotel_name → all(hotel.room_count) and time < 1 days
//map<string, deque<pair<time, room_count>>> timeRoomCountDequeHotelMap
//map<string, SUM(room_count)>               roomCountMap
//CLIENTS hotel_name → all(hotel.clients) and time < 1 days
//map<string, deque<pair<time, client_id>>> timeClientDequeHotelMap
//map<string, set<client_id>>               clientSetHotelMap

class HotelBookSystem
{

};

int main() {
    // Для ускорения чтения данных отключается синхронизация
    // cin и cout с stdio,
    // а также выполняется отвязка cin от cout
//    ios::sync_with_stdio(false);
//    cin.tie(nullptr);

    [[maybe_unused]] HotelBookSystem manager;

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
            //manager.Read(user_id, page_count);
        } else if (query_type == "CHEER") {
            //cout << setprecision(6) << manager.Cheer(user_id) << "\n";
        }
    }

    return 0;
}