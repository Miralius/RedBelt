//
// Created by F-Mir on 4/14/2024.
//
#include "stats.h"

const set<string> Stats::_methods = {"GET", "PUT", "POST", "DELETE", "UNKNOWN"};
const set<string> Stats::_uris = {"/", "/order", "/product", "/basket", "/help", "unknown"};

Stats::Stats()
{
    for (const auto& method: _methods)
    {
        _methodStats[method];
    }
    for (const auto& uri: _uris)
    {
        _uriStats[uri];
    }
}

void Stats::AddMethod(string_view method)
{
    if (auto methodIt = _methodStats.find(method); methodIt != _methodStats.end())
    {
        ++methodIt->second;
    } else
    {
        ++_methodStats["UNKNOWN"];
    }
}

void Stats::AddUri(string_view uri)
{
    if (auto uriIt = _uriStats.find(uri); uriIt != _uriStats.end())
    {
        ++uriIt->second;
    } else
    {
        ++_uriStats["unknown"];
    }
}

const map<string_view, int>& Stats::GetMethodStats() const
{
    return _methodStats;
}

const map<string_view, int>& Stats::GetUriStats() const
{
    return _uriStats;
}

string_view getAndCutWord(string_view& line)
{
    string_view result;
    size_t space = line.find(' ');
    while (space == 0 and !line.empty())
    {
        line.remove_prefix(1);
        space = line.find(' ');
    }
    result = line.substr(0, space);
    line.remove_prefix(space + 1);
    return result;
}

HttpRequest ParseRequest(string_view line)
{
    HttpRequest result;
    result.method = getAndCutWord(line);
    result.uri = getAndCutWord(line);
    result.protocol = line;
    return result;
}