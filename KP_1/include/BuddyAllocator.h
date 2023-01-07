#ifndef KP18_BUDDYALLOCATOR_H
#define KP18_BUDDYALLOCATOR_H
#include <vector>
#include <iostream>

class BuddyAllocator {
public:
    BuddyAllocator(const size_t allowedSize);

    ~BuddyAllocator();
    void* allocate(size_t mem_size);
    void deallocate(void *ptr);
    void PrintStatus(std::ostream& os) const;

private:
    std::vector<char*> freeBlocks;
    char* data;
    size_t mem_size;
};
#endif //KP18_BUDDYALLOCATOR_H
