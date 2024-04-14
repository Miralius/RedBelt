#pragma once

#include "http_request.h"

#include <string_view>
#include <string>
#include <map>
#include <set>

using namespace std;

class Stats
{
public:
    Stats();

    void AddMethod(string_view method);

    void AddUri(string_view uri);

    [[nodiscard]] const map<string_view, int>& GetMethodStats() const;

    [[nodiscard]] const map<string_view, int>& GetUriStats() const;

private:
    static const set<string> _methods;
    static const set<string> _uris;
    map<string_view, int> _methodStats;
    map<string_view, int> _uriStats;
};

HttpRequest ParseRequest(string_view line);
