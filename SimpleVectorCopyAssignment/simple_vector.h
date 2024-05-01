#pragma once

#include <cstdlib>
#include <algorithm>

// Реализуйте шаблон SimpleVector
template<typename T>
class SimpleVector
{
public:
    SimpleVector()
            : _data(nullptr), _capacity(0), _size(0) {}

    explicit SimpleVector(size_t size)
            : _data(new T[size]), _capacity(size), _size(size) {}

    SimpleVector& operator=(const SimpleVector& obj)
    {
        if (this != &obj)
        {
            T* newData = new T[obj.Size()];
            std::copy(obj.begin(), obj.end(), newData);
            if (_capacity > 0)
            {
                delete[] _data;
            }
            _data = newData;
            _capacity = obj.Size();
            _size = obj.Size();
        }
        return *this;
    }

    ~SimpleVector()
    {
        delete[] _data;
    }

    T& operator[](size_t index)
    {
        return _data[index];
    }

    T* begin()
    {
        return _data;
    }

    T* end()
    {
        return _data + _size;
    }

    const T* begin() const
    {
        return _data;
    }

    const T* end() const
    {
        return _data + _size;
    }

    [[nodiscard]] size_t Size() const
    {
        return _size;
    }

    [[nodiscard]] size_t Capacity() const
    {
        return _capacity;
    }

    [[maybe_unused]] void PushBack(const T& value)
    {
        if (_size >= _capacity)
        {
            auto newCapacity = _capacity == 0 ? 1 : 2 * _capacity;
            auto newData = new T[newCapacity];
            std::copy(begin(), end(), newData);
            delete[] _data;
            _data = newData;
            _capacity = newCapacity;
        }
        _data[_size++] = value;
    }

private:
    T* _data;
    size_t _capacity;
    size_t _size;
};
