#pragma once

#include <iostream>
#include <exception>
#include <map>

class BuddyAlloc {
    public:
        using MapType = std::multimap<size_t, char*>;

        size_t getFreeSize() {
            return maxSize; 
        }

        char* getAllocBlocks() { 
            return allocatedBlocks; 
        }

        MapType getFreeBlocks() { 
            return allocMap; 
        }

        BuddyAlloc(size_t minBlockSizeArg, size_t maxSizeArg) {
            minBlockSize = minBlockSizeArg;
            maxSize = maxSizeArg;
            allocatedBlocks = static_cast<char*>(::operator new(maxSize));
            allocMap.insert({maxSize, allocatedBlocks});
        }

        ~BuddyAlloc() {
            ::operator delete(allocatedBlocks);
            allocMap.clear();
        }

        void* allocate(const size_t size) {
            if (size == 0) {
                return nullptr;
            }

            //size_t roundUp = Rounder(size);

            for (MapType::iterator start = allocMap.begin(), end = allocMap.end(); start != end; ++start) {
                if (start->first == size) { 
                    void* result {start->second};
                    allocMap.erase(start);

                    return result;
                }
                else if (start->first > size) { 
                    int allocSize = start->first;
                    char* blockStart = start->second;

                    while (allocSize > int(size)) { 
                        allocSize /= 2; 
        
                        if (allocSize < int(size)) { 
                            break; 
                        }

                        allocMap.insert(std::pair<size_t, char*> (allocSize, blockStart + allocSize));
                    }
                    
                    allocMap.erase(start);

                    return blockStart;
                }
            }

            throw std::bad_alloc();
        }
        
        void deallocate(void* pointer, size_t size) {
            size_t bufferedSize = size;

            while(true) {
                std::pair<MapType::iterator, MapType::iterator> range (allocMap.equal_range(bufferedSize));

                if (range.first != allocMap.end() && range.first->first == bufferedSize) { 
                    auto start = range.first;
                    short index = ((char*)pointer - allocatedBlocks) / bufferedSize % 2; 
                    
                    while (start != range.second) {
                        if (index == 1) {
                            if (((char*)pointer - start->second) == int(bufferedSize)) { 
                                bufferedSize *= 2;
                                pointer = start->second;

                                allocMap.erase(start);
                                break; 
                            }
                        }
                        else {
                            if ((start->second - (char*)pointer) == int(bufferedSize)) {
                                bufferedSize *= 2;
                                allocMap.erase(start);
                                break; 
                            }
                        }

                        ++start;
                    }

                    if (start == range.second) { 
                        allocMap.insert(std::pair<size_t, char*> (bufferedSize, (char*)pointer));
                        break;
                    }
                    else {
                        continue;
                    }
                    
                }
                else {
                    allocMap.insert(std::pair<size_t, char*> (bufferedSize, (char*)pointer));
                    break;
                }
            }

        }

        size_t MaxAlloc() {
            if (allocMap.size() == 0) {
                return 0;
            }

            return (--allocMap.end())->first;
        }

    private:
        size_t Rounder(const size_t size) {
            if (size <= minBlockSize) {
                return minBlockSize;
            }

            size_t roundUp = 1;

            while (roundUp < size) { 
                roundUp <<= 1; 
            }

            return roundUp;
        }

    size_t minBlockSize, maxSize;
    MapType allocMap;
    char* allocatedBlocks;
};
