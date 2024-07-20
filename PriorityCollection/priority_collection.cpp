#if __GNUC__ < 13
#define STEPIK_CHECK_SYSTEM
#endif

#ifndef STEPIK_CHECK_SYSTEM

#include "../TestRunner/TestRunner.h"

#else
#include "test_runner.h"
#endif

#include <iostream>
#include <iterator>
#include <utility>
#include <list>
#include <vector>
#include <map>
#include <unordered_map>

using namespace std;

template<typename T>
class PriorityCollection
{
    struct PriorityObj;
    using Priority = unsigned;
public:
    using Id = unsigned;

    // Добавить объект с нулевым приоритетом
    // с помощью перемещения и вернуть его идентификатор
    Id Add(T object)
    {
        const Id newId = _counter++;
        if (_objectsByPriorities.empty())
        {
            _objectsByPriorities.emplace_back();
        }
        auto it = _objectsByPriorities.begin()->emplace(newId, PriorityObj(std::move(object))).first;
        _objectsById.emplace(newId, it);
        return newId;
    }

    // Добавить все элементы диапазона [range_begin, range_end)
    // с помощью перемещения, записав выданные им идентификаторы
    // в диапазон [ids_begin, ...)
    template<typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end,
             IdOutputIt ids_begin)
    {
        auto range_begin_move = std::make_move_iterator(range_begin);
        auto range_end_move = std::make_move_iterator(range_end);
        while (range_begin_move != range_end_move)
        {
            *ids_begin++ = Add(*range_begin_move++);
        }
    }

    // Определить, принадлежит ли идентификатор какому-либо
    // хранящемуся в контейнере объекту
    [[maybe_unused]] [[nodiscard]] bool IsValid(Id id) const
    {
        return _objectsById.find(id) != _objectsById.cend();
    }

    // Получить объект по идентификатору
    [[maybe_unused]] [[nodiscard]] const T& Get(Id id) const
    {
        return _objectsById.find(id)->second->second.obj;
    }

    // Увеличить приоритет объекта на 1
    void Promote(Id id)
    {
        auto objMapItIt = _objectsById.find(id);
        auto objMapIt = objMapItIt->second;
        auto obj = std::move(objMapIt->second);
        Priority priority = obj.priority;
        _objectsByPriorities[priority].erase(objMapIt);
        if (++priority == _objectsByPriorities.size())
        {
            _objectsByPriorities.emplace_back();
        }
        ++obj.priority;
        auto promotedIt = _objectsByPriorities[priority].emplace(id, std::move(obj)).first;
        _objectsById.erase(objMapItIt);
        _objectsById.emplace(id, promotedIt);
    }

    // Получить объект с максимальным приоритетом и его приоритет
    [[maybe_unused]] [[nodiscard]] pair<const T&, int> GetMax() const
    {
        // Через вектор приоритетов получаем доступ к объекту с максимальным приоритетом, который хранится в другой мапе
        const auto& maxPriorityMap = _objectsByPriorities.back();
        const auto& maxPriorityObj = maxPriorityMap.cbegin()->second;
        return {maxPriorityObj.obj, maxPriorityObj.priority };
    }

    // Аналогично GetMax, но удаляет элемент из контейнера
    pair<T, int> PopMax()
    {
        auto& maxPriorityMap = _objectsByPriorities.back();
        const Priority oldPriority = _objectsByPriorities.size() - 1;
        auto maxPriorityObjIt = maxPriorityMap.begin();
        auto maxPriorityObj = std::move(maxPriorityObjIt->second);
        _objectsById.erase(maxPriorityObjIt->first);
        maxPriorityMap.erase(maxPriorityObjIt);
        while (_objectsByPriorities.back().empty())
        {
            _objectsByPriorities.resize(_objectsByPriorities.size() - 1);
            if (_objectsByPriorities.empty())
            {
                break;
            }
        }
        return { std::move(maxPriorityObj.obj), oldPriority };
    }

private:
    Id _counter{};

    struct PriorityObj
    {
        explicit PriorityObj(T object)
        : obj(std::move(object))
        {}
        T obj;
        unsigned priority = 0;
    };

    using PriorityMap = std::map<Id, PriorityObj, std::greater<>>;
    std::unordered_map<Id, typename PriorityMap::iterator> _objectsById;
    // индексы вектора считаем приоритетами
    std::vector<PriorityMap> _objectsByPriorities;
};


class StringNonCopyable : public string
{
public:
    using string::string;  // Позволяет использовать конструкторы строки
    StringNonCopyable(const StringNonCopyable&) = delete;

    StringNonCopyable(StringNonCopyable&&) = default;

    StringNonCopyable& operator=(const StringNonCopyable&) = delete;

    StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy()
{
    PriorityCollection<StringNonCopyable> strings;
    strings.Add("white");
    const auto yellow_id = strings.Add("yellow");
    const auto red_id = strings.Add("red");

    strings.Promote(yellow_id);
    for (int i = 0; i < 2; ++i)
    {
        strings.Promote(red_id);
    }
    strings.Promote(yellow_id);
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "red")
        ASSERT_EQUAL(item.second, 2)
    }
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "yellow")
        ASSERT_EQUAL(item.second, 2)
    }
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "white")
        ASSERT_EQUAL(item.second, 0)
    }
}

void TestNoCopyList()
{
    PriorityCollection<StringNonCopyable> strings;
    std::list<StringNonCopyable> stringList;
    stringList.insert(stringList.end(), "white");
    stringList.insert(stringList.end(), "yellow");
    stringList.insert(stringList.end(), "red");

    std::vector<PriorityCollection<StringNonCopyable>::Id> ids;
    strings.Add(stringList.begin(), stringList.end(), std::back_inserter(ids));
    const auto yellow_id = ids.at(1);
    const auto red_id = ids.at(2);

    strings.Promote(yellow_id);
    for (int i = 0; i < 2; ++i)
    {
        strings.Promote(red_id);
    }
    strings.Promote(yellow_id);
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "red")
        ASSERT_EQUAL(item.second, 2)
    }
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "yellow")
        ASSERT_EQUAL(item.second, 2)
    }
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "white")
        ASSERT_EQUAL(item.second, 0)
    }
}

int main()
{
    TestRunner tr;
    RUN_TEST(tr, TestNoCopy);
    RUN_TEST(tr, TestNoCopyList);
    return 0;
}
