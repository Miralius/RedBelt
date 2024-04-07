//
// Created by F-Mir on 4/7/2024.
//
#include <iostream>
#include <cstdint>
#include <map>
#include <deque>

using namespace std;
using TimePoint = int64_t;
using HotelName = string;
using ClientId = uint32_t;
using RoomCount = uint16_t;

class HotelBookSystem
{
    static constexpr int64_t historyDuration = 60 * 60 * 24;
    using RoomCountSum = uint64_t;
public:
    void Book(const TimePoint& time, const HotelName& hotelName, const ClientId& clientId, const RoomCount& roomCount)
    {
        adjustRoomCount(time, hotelName, roomCount);
        adjustClients(time, hotelName, clientId);
    }
private:
    void adjustRoomCount(const TimePoint& time, const HotelName& hotelName, const RoomCount& roomCount)
    {
        auto& timeRoomCountDeque = _timeRoomCountDequeMap[hotelName];
        timeRoomCountDeque.emplace_back(time, roomCount);
        auto& roomCountSum = _roomCountMap[hotelName];
        roomCountSum += roomCount;
        while (timeRoomCountDeque.front().first + historyDuration <= time)
        {
            roomCountSum -= timeRoomCountDeque.front().second;
            timeRoomCountDeque.pop_front();
        }
    }

    void adjustClients(const TimePoint& time, const HotelName& hotelName, const ClientId& clientId)
    {
        auto& timeClientIdDeque = _timeClientIdDequeMap[hotelName];
        timeClientIdDeque.emplace_back(time, clientId);
        auto& hotelClients = _clientIdCounterDoubleMap[hotelName];
        hotelClients[clientId]++;
        while (timeClientIdDeque.front().first + historyDuration <= time)
        {
            const auto frontClientId = timeClientIdDeque.front().second;
            auto clientIt = hotelClients.find(frontClientId);
            if (--clientIt->second == 0)
            {
                hotelClients.erase(clientIt);
            }
            timeClientIdDeque.pop_front();
        }
    }

    map<HotelName, deque<pair<TimePoint, RoomCount>>> _timeRoomCountDequeMap;
    map<HotelName, RoomCountSum> _roomCountMap;
    map<HotelName, deque<pair<TimePoint, ClientId>>> _timeClientIdDequeMap;
    map<HotelName, map<ClientId, size_t /* counter */>> _clientIdCounterDoubleMap;
};

int main() {
    // Для ускорения чтения данных отключается синхронизация
    // cin и cout с stdio,
    // а также выполняется отвязка cin от cout
//    ios::sync_with_stdio(false);
//    cin.tie(nullptr);

    HotelBookSystem system;

    int queryCount;
    cin >> queryCount;

    for (int queryId = 0; queryId < queryCount; ++queryId) {
        string queryType;
        cin >> queryType;

        if (queryType == "BOOK") {
            TimePoint time;
            cin >> time;
            HotelName hotelName;
            cin >> hotelName;
            ClientId clientId;
            cin >> clientId;
            RoomCount roomCount;
            cin >> roomCount;
            system.Book(time, hotelName, clientId, roomCount);
        }
    }

    return 0;
}