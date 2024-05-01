#if __GNUC__ != 13 || __GNUC_MINOR__ != 2
#define STEPIK_CHECK_SYSTEM
#endif

#ifndef STEPIK_CHECK_SYSTEM

#include "../TestRunner/TestRunner.h"

#else
#include "test_runner.h"
#endif

#include <vector>

using namespace std;

// Объявляем Sentence<Token> для произвольного типа Token
// синонимом vector<Token>.
// Благодаря этому в качестве возвращаемого значения
// функции можно указать не малопонятный вектор векторов,
// а вектор предложений — vector<Sentence<Token>>.
template<typename Token>
using Sentence = vector<Token>;

// Класс Token имеет метод bool IsEndSentencePunctuation() const
template<typename Token>
vector<Sentence<Token>> SplitIntoSentences(vector<Token> tokens)
{
    vector<Sentence<Token>> result;
    Sentence<Token> sentence;
    bool wasEndSentencePunctuation = false;
    for (auto& token : tokens)
    {
        bool isEndSentencePunctuation = token.IsEndSentencePunctuation();
        if (!isEndSentencePunctuation and wasEndSentencePunctuation)
        {
            result.emplace_back(std::move(sentence));
        }
        sentence.emplace_back(std::move(token));
        wasEndSentencePunctuation = isEndSentencePunctuation;
    }
    if (!sentence.empty())
    {
        result.emplace_back(std::move(sentence));
    }
    return result;
}


struct TestToken
{
    string data;
    bool is_end_sentence_punctuation = false;

    [[nodiscard]] bool IsEndSentencePunctuation() const
    {
        return is_end_sentence_punctuation;
    }

    bool operator==(const TestToken& other) const
    {
        return data == other.data && is_end_sentence_punctuation == other.is_end_sentence_punctuation;
    }
};

ostream& operator<<(ostream& stream, const TestToken& token)
{
    return stream << token.data;
}

// Тест содержит копирования объектов класса TestToken.
// Для проверки отсутствия копирований в функции SplitIntoSentences
// необходимо написать отдельный тест.
void TestSplitting()
{
    ASSERT_EQUAL(
            SplitIntoSentences(vector<TestToken>({{"Split"},
                                                  {"into"},
                                                  {"sentences"},
                                                  {"!"}})),
            vector<Sentence<TestToken>>({
                                                {{"Split"}, {"into"}, {"sentences"}, {"!"}}
                                        })
    )

    ASSERT_EQUAL(
            SplitIntoSentences(vector<TestToken>({{"Split"},
                                                  {"into"},
                                                  {"sentences"},
                                                  {"!", true}})),
            vector<Sentence<TestToken>>({
                                                {{"Split"}, {"into"}, {"sentences"}, {"!", true}}
                                        })
    )

    ASSERT_EQUAL(
            SplitIntoSentences(vector<TestToken>({{"Split"},
                                                  {"into"},
                                                  {"sentences"},
                                                  {"!", true},
                                                  {"!", true},
                                                  {"Without"},
                                                  {"copies"},
                                                  {".", true}})),
            vector<Sentence<TestToken>>({
                                                {{"Split"},   {"into"},   {"sentences"}, {"!", true}, {"!", true}},
                                                {{"Without"}, {"copies"}, {".", true}},
                                        })
    )
}

int main()
{
    TestRunner tr;
    RUN_TEST(tr, TestSplitting);
    return 0;
}
