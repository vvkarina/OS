#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include "buddy_allocator.h"

using TAllocPair = std::pair<size_t, char*>;

void Test(BuddyAlloc &buddyAllocator, unsigned int memorySize) {
    const int blockSizes[] = {32, 64, 128, 256, 512};
    int maxSize = 1024, blockSize;
    TAllocPair pair;
    std::vector<TAllocPair> vector;
    auto timeBegin = std::chrono::high_resolution_clock::now();

    for (int index = 0; index < int(memorySize); ++index) {
        maxSize = buddyAllocator.MaxAlloc();

        if (maxSize == 0) {
            auto iterator = vector.begin() + std::rand() % vector.size();
            buddyAllocator.deallocate(iterator->second, iterator->first);
            maxSize += iterator->first;   
            vector.erase(iterator);
        }
        else if (maxSize == 1024) {
            blockSize = blockSizes[std::rand() % 5];
            vector.push_back(std::pair<size_t, char*> (blockSize, (char*)buddyAllocator.allocate(blockSize)));
            maxSize -= blockSize;  
        }
        else {
            if (std::rand() % 2) {
                if (maxSize >= 512) { 
                    blockSize = blockSizes[std::rand() % 5]; 
                }
                else if (maxSize >= 256) { 
                    blockSize = blockSizes[std::rand() % 4]; 
                }
                else if (maxSize >= 128) { 
                    blockSize = blockSizes[std::rand() % 3]; 
                }
                else if (maxSize >= 64 ) { 
                    blockSize = blockSizes[std::rand() % 2]; 
                }
                else if (maxSize >= 32 ) { 
                    blockSize = blockSizes[0]; 
                }

                vector.push_back(std::pair<size_t, char*> (blockSize, (char*)buddyAllocator.allocate(blockSize)));
                maxSize -= blockSize; 
            }
            else {
                auto iterator = vector.begin() + std::rand() % vector.size();
                buddyAllocator.deallocate(iterator->second, iterator->first);
                maxSize += iterator->first;  
                vector.erase(iterator);
            }
            
        }   
    }

    auto timeEnd = std::chrono::high_resolution_clock::now();
    std::cout << "Allocation time by buddyAllocator for memory_size = " << memorySize << " is " << std::chrono::duration<double>(timeEnd - timeBegin).count() << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Write memory_size" << std::endl;

        return -1;
    }

    BuddyAlloc buddyAllocator(32, 1024);

    Test(buddyAllocator, std::atoi(argv[1]));

    return 0;
}
