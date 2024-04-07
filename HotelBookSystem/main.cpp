//
// Created by F-Mir on 4/7/2024.
//
#include <iostream>
#include <cstdint>
#include <map>
#include <deque>
#include <limits>

using namespace std;
using TimePoint = int64_t;
using HotelName = string;
using ClientId = uint32_t;
using RoomCount = uint16_t;

class HotelBookSystem {
    static constexpr int64_t historyDuration = 60 * 60 * 24;
    using RoomCountSum = uint64_t;
public:
    void Book(const TimePoint& time, const HotelName& hotelName, const ClientId& clientId, const RoomCount& roomCount) {
        _timeRoomCountDequeMap[hotelName].emplace_back(time, roomCount);
        _roomCountMap[hotelName] += roomCount;
        _timeClientIdDequeMap[hotelName].emplace_back(time, clientId);
        _clientIdCounterDoubleMap[hotelName][clientId]++;
        _lastTime = time;
    }

    [[nodiscard]] uint32_t Clients(const HotelName& hotelName) {
        adjustClients(hotelName);
        const auto hotelIt = _clientIdCounterDoubleMap.find(hotelName);
        if (hotelIt != _clientIdCounterDoubleMap.cend()) {
            return hotelIt->second.size();
        }
        return 0;
    }

    [[nodiscard]] RoomCountSum Rooms(const HotelName& hotelName) {
        adjustRoomCount(hotelName);
        const auto hotelIt = _roomCountMap.find(hotelName);
        if (hotelIt != _roomCountMap.cend()) {
            return hotelIt->second;
        }
        return 0;
    }

private:
    void adjustRoomCount(const HotelName& hotelName) {
        auto& timeRoomCountDeque = _timeRoomCountDequeMap[hotelName];
        auto& roomCountSum = _roomCountMap[hotelName];
        while (!timeRoomCountDeque.empty() and timeRoomCountDeque.front().first + historyDuration <= _lastTime) {
            roomCountSum -= timeRoomCountDeque.front().second;
            timeRoomCountDeque.pop_front();
        }
    }

    void adjustClients(const HotelName& hotelName) {
        auto& timeClientIdDeque = _timeClientIdDequeMap[hotelName];
        auto& hotelClients = _clientIdCounterDoubleMap[hotelName];
        while (!timeClientIdDeque.empty() and timeClientIdDeque.front().first + historyDuration <= _lastTime) {
            const auto frontClientId = timeClientIdDeque.front().second;
            auto clientIt = hotelClients.find(frontClientId);
            if (--clientIt->second == 0) {
                hotelClients.erase(clientIt);
            }
            timeClientIdDeque.pop_front();
        }
    }

    map<HotelName, deque<pair<TimePoint, RoomCount>>> _timeRoomCountDequeMap;
    map<HotelName, RoomCountSum> _roomCountMap;
    map<HotelName, deque<pair<TimePoint, ClientId>>> _timeClientIdDequeMap;
    map<HotelName, map<ClientId, size_t /* counter */>> _clientIdCounterDoubleMap;
    TimePoint _lastTime = numeric_limits<int64_t>::min();
};

int main() {
    // Для ускорения чтения данных отключается синхронизация
    // cin и cout с stdio,
    // а также выполняется отвязка cin от cout
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    HotelBookSystem system;

    int queryCount;
    cin >> queryCount;

    try {
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
            } else if (queryType == "CLIENTS") {
                HotelName hotelName;
                cin >> hotelName;
                cout << system.Clients(hotelName) << '\n';
            } else if (queryType == "ROOMS") {
                HotelName hotelName;
                cin >> hotelName;
                cout << system.Rooms(hotelName) << '\n';
            }
        }
    }
    catch (const exception& ex) {
        cerr << "Exception! " << ex.what();
    }

    return 0;
}