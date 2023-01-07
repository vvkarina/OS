#include <iostream>
#include <chrono>

#include "N2Allocator.h"
#include "BuddyAllocator.h"

int main() {
    using namespace std::chrono;
    {
        steady_clock::time_point buddy_allocator_init_start = steady_clock::now();
        BuddyAllocator buddy_allocator(4096);
        steady_clock::time_point buddy_allocator_init_end = steady_clock::now();
        std::cerr << "Buddy allocator initialization with one page of memory :"
                  << std::chrono::duration_cast<std::chrono::nanoseconds>(
                          buddy_allocator_init_end - buddy_allocator_init_start).count()
                  << " ns" << std::endl;


        steady_clock::time_point n2_allocator_init_start = steady_clock::now();
        N2Allocator n2_allocator(
            {64, 32, 16,4, 2}); // 1 страница
        steady_clock::time_point n2_allocator_init_end = steady_clock::now();
        std::cerr << "N2 allocator initialization with one page of memory :"
                  << std::chrono::duration_cast<std::chrono::nanoseconds>(
                          n2_allocator_init_end - n2_allocator_init_start).count()
                  << " ns" << std::endl;

        std::cerr << "\n";

    }
    std::cerr << "First test: Allocate 10 char[256] arrays, free 5 of them, allocate 10 char[128] arrays:\n";
    {
        std::vector<char *> pointers(15, 0);
        N2Allocator allocator(
                {0, 0,20, 20, 20, 10});
        steady_clock::time_point n2_test1_start = steady_clock::now();
        for (int i = 0; i < 10; ++i) {
            pointers[i] = (char *) allocator.allocate(256);
        }
        for (int i = 5; i < 10; ++i) {
            allocator.deallocate(pointers[i]);
        }
        for (int i = 5; i < 15; ++i) {
            pointers[i] = (char *) allocator.allocate(128);
        }
        steady_clock::time_point n2_test1_end = steady_clock::now();
        std::cerr << "N2 allocator first test:"
                  << std::chrono::duration_cast<std::chrono::microseconds>(n2_test1_end - n2_test1_start).count()
                  << " microseconds" << std::endl;
        allocator.PrintStatus(std::cerr);
        for (int i = 0; i < 15; ++i) {
            allocator.deallocate(pointers[i]);
        }
    }
    {
        BuddyAllocator allocator(8192);
        std::vector<char *> pointers(1000, 0);
        steady_clock::time_point test1_start = steady_clock::now();
        for (int i = 0; i < 10; ++i) {
            pointers[i] = (char *) allocator.allocate(256);
        }
        for (int i = 5; i < 10; ++i) {
            allocator.deallocate(pointers[i]);
        }
        for (int i = 5; i < 15; ++i) {
            pointers[i] = (char *) allocator.allocate(128);
        }
        steady_clock::time_point test1_end = steady_clock::now();
        std::cout << "Buddy allocator first test:"
                  << std::chrono::duration_cast<std::chrono::microseconds>(test1_end - test1_start).count()
                  << " microseconds" << std::endl;
        allocator.PrintStatus(std::cerr);
        for (int i = 0; i < 15; ++i) {
            allocator.deallocate(pointers[i]);
        }
    }
    std::cerr << "Second test: Allocate and free 75 20 bytes arrays:\n";
    {
        N2Allocator allocator({0,400,400});
        std::vector<char *> pointers(75, 0);
        steady_clock::time_point alloc_start = steady_clock::now();
        for (int i = 0; i < 75; ++i) {
            pointers[i] = (char *) allocator.allocate(20);
        }
        steady_clock::time_point alloc_end = steady_clock::now();
        for (int i = 0; i < 75; ++i) {
            allocator.deallocate(pointers[i]);
        }
        steady_clock::time_point test_end = steady_clock::now();
        std::cerr << "N2 allocator second test:\n"
                  << "Allocation :" << duration_cast<std::chrono::microseconds>(alloc_end - alloc_start).count() << " microseconds" << "\n"
                  << "Deallocation :" << duration_cast<std::chrono::microseconds>(test_end - alloc_end).count() << " microseconds" << "\n";
    }
    {
        BuddyAllocator allocator(16000);
        std::vector<char *> pointers(75, 0);
        steady_clock::time_point alloc_start = steady_clock::now();
        for (int i = 0; i < 75; ++i) {
            pointers[i] = (char *) allocator.allocate(20);
        }
        steady_clock::time_point alloc_end = steady_clock::now();
        for (int i = 0; i < 75; ++i) {
            allocator.deallocate(pointers[i]);
        }
        steady_clock::time_point test_end = steady_clock::now();
        std::cerr << "Buddy allocator second test:\n"
                  << "Allocation :" << duration_cast<std::chrono::microseconds>(alloc_end - alloc_start).count() << " microseconds" << "\n"
                  << "Deallocation :" << duration_cast<std::chrono::microseconds>(test_end - alloc_end).count() << " microseconds" << "\n";
    }
    std::cerr << "Third test: Allocate 50 20 bytes arrays, deallocate every second, allocate 25 12 bytes :\n";
    {
        N2Allocator allocator({400, 700});
        std::vector<char *> pointers(75, 0);
        steady_clock::time_point test_start = steady_clock::now();
        for (int i = 0; i < 50; ++i) {
            pointers[i] = (char *) allocator.allocate(20);
        }
        for (int i = 0; i < 25; ++i) {
            allocator.deallocate(pointers[i * 2]);
        }
        for (int i = 500; i < 75; ++i) {
            pointers[i] = (char*) allocator.allocate(12);
        }
        steady_clock::time_point test_end = steady_clock::now();
        std::cerr << "N2 allocator third test:"
                  << std::chrono::duration_cast<std::chrono::microseconds>(test_end - test_start).count()
                  << " microseconds" << std::endl;
        allocator.PrintStatus(std::cerr);
        for (int i = 0; i < 25; ++i) {
            allocator.deallocate(pointers[i * 2 + 1]);
        }
        for (int i = 500; i < 75; ++i) {
            allocator.deallocate(pointers[i]);
        }
    }
    {
        BuddyAllocator allocator(16000);
        std::vector<char *> pointers(75, 0);
        steady_clock::time_point test_start = steady_clock::now();
        for (int i = 0; i < 50; ++i) {
            pointers[i] = (char *) allocator.allocate(20);
        }
        for (int i = 0; i < 25; ++i) {
            allocator.deallocate(pointers[i * 2]);
        }
        for (int i = 500; i < 75; ++i) {
            pointers[i] = (char*) allocator.allocate(12);
        }
        steady_clock::time_point test_end = steady_clock::now();
        std::cerr << "Buddy allocator third test:"
                  << std::chrono::duration_cast<std::chrono::microseconds>(test_end - test_start).count()
                  << " microseconds" << std::endl;
        allocator.PrintStatus(std::cerr);
        for (int i = 0; i < 25; ++i) {
            allocator.deallocate(pointers[i * 2 + 1]);
        }
        for (int i = 500; i < 75; ++i) {
            allocator.deallocate(pointers[i]);
        }
    }
    std::cerr << "Fourth test: Allocate and free 150 20 bytes arrays:\n";
    {
        N2Allocator allocator({0, 800, 800});
        std::vector<char *> pointers(150, 0);
        steady_clock::time_point alloc_start = steady_clock::now();
        for (int i = 0; i < 150; ++i) {
            pointers[i] = (char *) allocator.allocate(20);
        }
        steady_clock::time_point alloc_end = steady_clock::now();
        for (int i = 0; i < 150; ++i) {
            allocator.deallocate(pointers[i]);
        }
        steady_clock::time_point test_end = steady_clock::now();
        std::cerr << "N2 allocator fourth test:\n"
                  << "Allocation :" << duration_cast<std::chrono::microseconds>(alloc_end - alloc_start).count() << " microseconds" << "\n"
                  << "Deallocation :" << duration_cast<std::chrono::microseconds>(test_end - alloc_end).count() << " microseconds" << "\n";
    }
    {
        BuddyAllocator allocator(32000);
        std::vector<char *> pointers(150, 0);
        steady_clock::time_point alloc_start = steady_clock::now();
        for (int i = 0; i < 150; ++i) {
            pointers[i] = (char *) allocator.allocate(20);
        }
        steady_clock::time_point alloc_end = steady_clock::now();
        for (int i = 0; i < 150; ++i) {
            allocator.deallocate(pointers[i]);
        }
        steady_clock::time_point test_end = steady_clock::now();
        std::cerr << "Buddy allocator fourth test:\n"
                  << "Allocation :" << duration_cast<std::chrono::microseconds>(alloc_end - alloc_start).count() << " microseconds" << "\n"
                  << "Deallocation :" << duration_cast<std::chrono::microseconds>(test_end - alloc_end).count() << " microseconds" << "\n";
    }

    return 0;
}