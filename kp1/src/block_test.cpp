#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <vector>
#include "block_allocator.h"

void Fill(size_t size, size_t amount, std::vector<size_t> &free) {
    for (int index = 0; index < int(amount); ++index) {
        free.push_back(size);
    }
}

void test(unsigned int memorySize, BlockAlloc &blockAllocator) {
    std::vector<size_t> free;
    std::vector< std::pair<void*, size_t> > allocated;

    Fill(24, 4, free);   
    Fill(56, 4, free);
    Fill(120, 4, free);
    Fill(248, 4, free);
    Fill(504, 2, free);
    Fill(1016, 1, free);

    auto timeBegin = std::chrono::high_resolution_clock::now();

    for (int index = 0; index < int(memorySize); ++index) {
        if (free.empty()) {
            auto iterator = allocated.begin() + std::rand() % allocated.size();

            free.push_back(iterator->second);
            blockAllocator.deallocate(iterator->first);
            allocated.erase(iterator);
        }
        else if (allocated.empty()) {
            auto iterator = free.begin() + std::rand() % free.size(); 
            void* pointer = blockAllocator.allocate(*iterator);

            if (pointer == NULL) { 
                throw std::bad_alloc();
            }

            allocated.push_back(std::pair<void*, size_t> (pointer, *iterator));
            free.erase(iterator);
        }
        else {
            if (std::rand() % 2) {
                auto iterator = free.begin() + std::rand() % free.size();

                allocated.push_back(std::pair<void*, size_t> (blockAllocator.allocate(*iterator), *iterator));
                free.erase(iterator);
            }
            else {
                auto iterator = allocated.begin() + std::rand() % allocated.size();
                
                free.push_back(iterator->second);
                blockAllocator.deallocate(iterator->first);
                allocated.erase(iterator);
            }
        }
    }

    auto timeEnd = std::chrono::high_resolution_clock::now();
    std::cout << "Allocation time by blockAllocator for memory_size = " << memorySize << " is " << std::chrono::duration<double>(timeEnd - timeBegin).count() << std::endl;
}

int main(int argc, char* argv[]) { 
    if (argc < 2) {
        std::cerr << "Write memory_size" << std::endl;

        return -1;
    }

    BlockAlloc blockAllocator;
    
    test(std::atoi(argv[1]), blockAllocator);

    return 0;
}
