// The #if is needed for stepik check system submitting if we use the whole file
#if __GNUC__ == 13 && __GNUC_MINOR__ == 2

#include "../TestRunner/TestRunner.h"

#else
#include "test_runner.h"
#endif

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>

using namespace std;

template<class T>
class ObjectPool {
public:
    T* Allocate() {
        if (!_deallocatedObjects.empty()) {
            return getDeallocatedObject();
        } else {
            return *(_allocatedObjects.emplace(new T).first);
        }
    }

    [[maybe_unused]] T* TryAllocate() {
        if (!_deallocatedObjects.empty()) {
            return getDeallocatedObject();
        }
        return nullptr;
    }

    void Deallocate(T* object) {
        auto allocatedObjectIt = _allocatedObjects.find(object);
        if (allocatedObjectIt != _allocatedObjects.cend()) {
            _deallocatedObjects.emplace(*allocatedObjectIt);
            _allocatedObjects.erase(allocatedObjectIt);
        } else {
            throw invalid_argument("The object didn't found in allocated object set");
        }
    }

    ~ObjectPool() {
        for (auto& allocatedObject: _allocatedObjects) {
            delete allocatedObject;
        }
        _allocatedObjects.clear();

        while (!_deallocatedObjects.empty()) {
            delete _deallocatedObjects.front();
            _deallocatedObjects.pop();
        }
    }

private:
    T* getDeallocatedObject() {
        auto deallocatedObject = _deallocatedObjects.front();
        _allocatedObjects.insert(deallocatedObject);
        _deallocatedObjects.pop();
        return deallocatedObject;
    }

    set<T*> _allocatedObjects;
    queue<T*> _deallocatedObjects;
};

void TestObjectPool() {
    ObjectPool<string> pool;

    auto p1 = pool.Allocate();
    auto p2 = pool.Allocate();
    auto p3 = pool.Allocate();

    *p1 = "first";
    *p2 = "second";
    *p3 = "third";

    pool.Deallocate(p2);
    ASSERT_EQUAL(*pool.Allocate(), "second")

    pool.Deallocate(p3);
    pool.Deallocate(p1);
    ASSERT_EQUAL(*pool.Allocate(), "third")
    ASSERT_EQUAL(*pool.Allocate(), "first")

    pool.Deallocate(p1);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestObjectPool);
    return 0;
}
