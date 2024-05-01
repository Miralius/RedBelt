#include <cstdint>
#include <algorithm>

// Реализуйте SimpleVector в этом файле
// и отправьте его на проверку

template<typename T>
class SimpleVector
{
public:
    SimpleVector() = default;

    explicit SimpleVector(size_t size)
            : _data(new T[size]), _capacity(size), _size(size) {}

    SimpleVector(const SimpleVector& obj)
            : _data(new T[obj._size]), _capacity(obj._size), _size(obj._size)
    {
        std::copy(obj.begin(), obj.end(), _data);
    }

    SimpleVector(SimpleVector&& obj) noexcept
            : _data(obj._data), _capacity(obj._size), _size(obj._size)
    {
        obj._data = nullptr;
        obj._capacity = 0;
        obj._size = 0;
    }

    SimpleVector& operator=(const SimpleVector& obj)
    {
        if (this != &obj)
        {
            if (obj._size <= _capacity)
            {
                std::copy(obj.begin(), obj.end(), _data);
                _size = obj._size;
            } else
            {
                SimpleVector<T> tmp(obj);
                swap(tmp);
            }
        }
        return *this;
    }

    SimpleVector& operator=(SimpleVector&& obj) noexcept
    {
        if (this != &obj)
        {
            swap(obj);
        }
        return *this;
    }

    ~SimpleVector()
    {
        if (_size != 0)
        {
            delete[] _data;
        }
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

    template<typename Value>
    [[maybe_unused]] void PushBack(Value&& value)
    {
        if (_size >= _capacity)
        {
            auto newCapacity = _capacity == 0 ? 1 : 2 * _capacity;
            auto newData = new T[newCapacity];
            std::move(begin(), end(), newData);
            if (_size != 0)
            {
                delete[] _data;
            }
            _data = newData;
            _capacity = newCapacity;
        }
        _data[_size++] = std::forward<Value>(value);
    }

private:
    void swap(SimpleVector& obj)
    {
        std::swap(_data, obj._data);
        std::swap(_capacity, obj._capacity);
        std::swap(_size, obj._size);
    }

    T* _data;
    size_t _capacity{};
    size_t _size{};
};
