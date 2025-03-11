#include <iostream>
#include <vector>

class FixedBlockMemoryPool {
public:
    FixedBlockMemoryPool(size_t blockSize, size_t blockCount)
        : blockSize(blockSize), blockCount(blockCount) {
        pool.resize(blockSize * blockCount);
        for (size_t i = 0; i < blockCount; ++i) {
            freeBlocks.push_back(&pool[i * blockSize]);
        }
    }

    void* allocate() {
        if (freeBlocks.empty()) {
            throw std::bad_alloc();
        }
        void* block = freeBlocks.back();
        freeBlocks.pop_back();
        return block;
    }

    void deallocate(void* block) {
        freeBlocks.push_back(block);
    }

private:
    size_t blockSize;
    size_t blockCount;
    std::vector<char> pool;
    std::vector<void*> freeBlocks;
};

int main() {
    const size_t blockSize = 32;
    const size_t blockCount = 10;

    FixedBlockMemoryPool memoryPool(blockSize, blockCount);

    void* block1 = memoryPool.allocate();
    void* block2 = memoryPool.allocate();

    std::cout << "Allocated blocks at: " << block1 << " and " << block2 << std::endl;

    memoryPool.deallocate(block1);
    memoryPool.deallocate(block2);

    std::cout << "Deallocated blocks." << std::endl;

    return 0;
}