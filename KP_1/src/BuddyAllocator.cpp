
#include "BuddyAllocator.h"
#include <algorithm>
void setBlock(char* p, size_t size) {
    *((int*) p) = size;
}

int getSize(char* p) {
    return *((int*) p);
}

BuddyAllocator::BuddyAllocator(const size_t allowedSize) : mem_size{allowedSize} {
    data = (char*)malloc(allowedSize);
    setBlock(data, allowedSize);
    freeBlocks.push_back(data);
}

BuddyAllocator::~BuddyAllocator() {
    free(data);
}

void *BuddyAllocator::allocate(size_t mem_size) {
    if (mem_size == 0) {
        return nullptr;
    }
    int index = -1;
    mem_size += sizeof(int);
    for (int i = 0; i < int(freeBlocks.size()); ++i) {
        if (getSize(freeBlocks[i]) >= int(mem_size)) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        throw std::bad_alloc();
    }
    size_t currentBlockSize = getSize(freeBlocks[index]);
    while ((currentBlockSize % 2 == 0) && (currentBlockSize / 2>= mem_size)) {
        currentBlockSize /= 2;
        char* newBlock = freeBlocks[index] + currentBlockSize;
        setBlock(newBlock, currentBlockSize);
        freeBlocks.push_back(newBlock);
    }
    setBlock(freeBlocks[index], currentBlockSize);
    freeBlocks.erase(std::next(freeBlocks.begin(), index));
    return freeBlocks[index] + sizeof(int);
}

void BuddyAllocator::deallocate(void *ptr) {
    char *c_ptr = (char*) ptr - sizeof(int);
    size_t size = getSize(c_ptr);
    auto found = std::find(freeBlocks.begin(), freeBlocks.end(), c_ptr + size);
    if (found != freeBlocks.end()) {
        freeBlocks.erase(found);
        setBlock(c_ptr, size * 2 );
        freeBlocks.push_back(c_ptr);
        return;
    }
    found = std::find(freeBlocks.begin(), freeBlocks.end(), c_ptr - size);
    if (found != freeBlocks.end()) {
        setBlock(c_ptr - size, size * 2);
        return;
    }
    freeBlocks.push_back(c_ptr);
}

void BuddyAllocator::PrintStatus(std::ostream &os) const {
    int free_sum = 0;
    for (auto block : freeBlocks) {
        free_sum += getSize(block);
    }
    int occ_sum = mem_size - free_sum;
    os << "Occupied memory: " << occ_sum << std::endl;
    os << "Free memory: " << free_sum << std::endl << std::endl;
}
