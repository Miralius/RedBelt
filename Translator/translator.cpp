#if __GNUC__ != 13 || __GNUC_MINOR__ != 2
#define STEPIK_CHECK_SYSTEM
#endif

#ifndef STEPIK_CHECK_SYSTEM

#include "../TestRunner/TestRunner.h"

#else
#include "test_runner.h"
#endif

#include <string>
#include <set>

using namespace std;

class Translator {
public:
    void Add(string_view source, string_view target)
    {
        auto sourceInSet = strings.emplace(source).first;
        auto targetInSet = strings.emplace(target).first;
        forwardTranslations[*sourceInSet] = *targetInSet;
        backwardTranslations[*targetInSet] = *sourceInSet;
    }

    [[nodiscard]] string_view TranslateForward(string_view source) const
    {
        return getTranslation(forwardTranslations, source);
    }

    [[nodiscard]] string_view TranslateBackward(string_view target) const
    {
        return getTranslation(backwardTranslations, target);
    }

private:
    using Dictionary = map<string_view, string_view>;
    static string_view getTranslation(const Dictionary& from, string_view what)
    {
        const auto resultIt = from.find(what);
        if (resultIt != from.cend())
        {
            return resultIt->second;
        }
        return {};
    }

    set<string> strings;
    Dictionary forwardTranslations;
    Dictionary backwardTranslations;
};

void TestSimple() {
    Translator translator;
    translator.Add(string("okno"), string("window"));
    translator.Add(string("stol"), string("table"));

    ASSERT_EQUAL(translator.TranslateForward("okno"), "window")
    ASSERT_EQUAL(translator.TranslateBackward("table"), "stol")
    ASSERT_EQUAL(translator.TranslateBackward("stol"), "")
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSimple);
    return 0;
}