#ifndef KP18_N2ALLOCATOR_H
#define KP18_N2ALLOCATOR_H
#include <vector>
#include <iostream>
#include <list>
struct N2AllocatorInit {
    unsigned int block_16 = 0;
    unsigned int block_32 = 0;
    unsigned int block_64 = 0;
    unsigned int block_128 = 0;
    unsigned int block_256 = 0;
    unsigned int block_512 = 0;
    unsigned int block_1024 = 0;
};

class N2Allocator {
public:
    N2Allocator(const N2AllocatorInit& init_data);

    ~N2Allocator();
    void* allocate(size_t mem_size);
    void deallocate(void *ptr);
    void PrintStatus(std::ostream& os) const;

private:
    const std::vector<int> index_to_size = {16, 32, 64, 128, 512, 1024};
    std::vector<std::list<char*>> lists;
    char* data;
    int mem_size;
};
#endif //KP18_N2ALLOCATOR_H
