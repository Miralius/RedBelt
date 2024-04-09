#pragma once

#include <stdexcept>

using namespace std;

template<typename T, size_t N>
class StackVector {
public:
    explicit StackVector(size_t a_size = 0)
            : size(a_size) {
        if (size > N) {
            throw invalid_argument("Too big a_size was specified");
        }
    }

    T& operator[](size_t index) {
        return data[index];
    }

    const T& operator[](size_t index) const {
        return data[index];
    }

    T* begin() {
        return data;
    }

    T* end() {
        return data + size;
    }

    const T* begin() const {
        return data;
    }

    const T* end() const {
        return data + size;
    }

    [[nodiscard]] size_t Size() const {
        return size;
    }

    [[nodiscard]] size_t Capacity() const {
        return N;
    }

    void PushBack(const T& value) {
        if (size == N) {
            throw overflow_error("Array is full");
        }
        data[size++] = value;
    }

    T PopBack() {
        if (size == 0) {
            throw underflow_error("Array is empty");
        }
        return data[--size];
    }

private:
    T data[N];
    size_t size;
};

