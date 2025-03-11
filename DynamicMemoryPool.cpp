#include <iostream>
#include <vector>

class MemoryPool {
public:
    MemoryPool(size_t chunkSize, size_t initialChunks = 32)
        : chunkSize(chunkSize), poolSize(initialChunks * chunkSize) {
        pool = new char[poolSize];
        freeList.reserve(initialChunks);
        for (size_t i = 0; i < initialChunks; ++i) {
            freeList.push_back(pool + i * chunkSize);
        }
    }

    ~MemoryPool() {
        delete[] pool;
    }

    void* allocate() {
        if (freeList.empty()) {
            expandPool();
        }
        void* ptr = freeList.back();
        freeList.pop_back();
        return ptr;
    }

    void deallocate(void* ptr) {
        freeList.push_back(static_cast<char*>(ptr));
    }

private:
    void expandPool() {
        size_t newPoolSize = poolSize * 2;
        char* newPool = new char[newPoolSize];
        for (size_t i = 0; i < poolSize; ++i) {
            newPool[i] = pool[i];
        }
        delete[] pool;
        pool = newPool;
        poolSize = newPoolSize;
        for (size_t i = poolSize / 2; i < poolSize; ++i) {
            freeList.push_back(pool + i * chunkSize);
        }
    }

    size_t chunkSize;
    size_t poolSize;
    char* pool;
    std::vector<void*> freeList;
};

int main() {
    MemoryPool pool(256);

    void* ptr1 = pool.allocate();
    void* ptr2 = pool.allocate();

    pool.deallocate(ptr1);
    pool.deallocate(ptr2);

    return 0;
}