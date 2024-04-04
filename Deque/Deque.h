//
// Created by F-Mir on 4/3/2024.
//

#ifndef RED_BELT_DEQUE_H
#define RED_BELT_DEQUE_H

#pragma once

#include <vector>

#define ACCESS_OPERATOR_BODY(index)                           \
{                                                             \
    if (index >= _frontVector.size())                         \
    {                                                         \
        return _backVector[index - _frontVector.size()];      \
    }                                                         \
    else                                                      \
    {                                                         \
        return _frontVector[_frontVector.size() - 1 - index]; \
    }                                                         \
}

#define AT_OPERATOR_BODY(index)                                  \
{                                                                \
    if (index >= _frontVector.size())                            \
    {                                                            \
        return _backVector.at(index - _frontVector.size());      \
    }                                                            \
    else                                                         \
    {                                                            \
        return _frontVector.at(_frontVector.size() - 1 - index); \
    }                                                            \
}

#define FRONT_BODY                                                           \
{                                                                            \
    return _frontVector.empty() ? _backVector.front() : _frontVector.back(); \
}

#define BACK_BODY                                                           \
{                                                                           \
    return _backVector.empty() ? _frontVector.front() : _backVector.back(); \
}

#define PUSH_BODY(vector, element)              \
{                                               \
    vector.push_back(std::forward<T>(element)); \
}

template<class T>
class Deque {
public:
    Deque() = default;

    [[nodiscard]] bool Empty() const {
        return _frontVector.empty() and _backVector.empty();
    }

    [[nodiscard]] size_t Size() const {
        return _frontVector.size() + _backVector.size();
    }

    T &operator[](size_t index) ACCESS_OPERATOR_BODY(index)

    const T &operator[](size_t index) const ACCESS_OPERATOR_BODY(index)

    T &At(size_t index) AT_OPERATOR_BODY(index)

    [[maybe_unused]] const T &At(size_t index) const AT_OPERATOR_BODY(index)

    T & Front() FRONT_BODY

    [[maybe_unused]] const T & Front() const FRONT_BODY

    T & Back() BACK_BODY

    [[maybe_unused]] const T & Back() const BACK_BODY

    void PushBack(T&& element) PUSH_BODY(_backVector, element)

    void PushFront(T&& element) PUSH_BODY(_frontVector, element)

private:
    std::vector<T> _frontVector;
    std::vector<T> _backVector;
};


#endif //RED_BELT_DEQUE_H
