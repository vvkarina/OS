#include <stdio.h>
#pragma once

#include <iostream>
#include <exception>

using RawByte = char*;

class BlockAlloc {
    public:
        BlockAlloc() {
            allMemory = static_cast<RawByte>(::operator new(  
                4 * 32 + 4 * 64 + 4 * 128 + 4 * 256 + 2 * 512 + 1 * 1024
            ));

            BindBlocks(32, 4, 0);
            list[0] = allMemory;

            BindBlocks(64, 4, 128);
            list[1] = allMemory + 128;
        
            BindBlocks(128, 4, 384);
            list[2] = allMemory + 384;

            BindBlocks(256, 4, 896);
            list[3] = allMemory + 896;

            BindBlocks(512, 2, 1920);
            list[4] = allMemory + 1920;

            BindBlocks(1024, 1, 2944);
            list[5] = allMemory + 2944;
        }

        ~BlockAlloc() {
            ::operator delete(allMemory);
        }

        void* allocate(size_t size) {
            size = Rounder(size);
            int index;

            switch (size) {
                case 32: { 
                    index = 0; 
                    break;
                }
                case 64: { 
                    index = 1; 
                    break;
                }
                case 128: { 
                    index = 2; 
                    break;
                }
                case 256: { 
                    index = 3; 
                    break;
                }
                case 512: { 
                    index = 4; 
                    break;
                }
                case 1024: { 
                    index = 5; 
                    break;
                }
                default: {
                    throw std::bad_alloc();
                }
            }

            for ( ; index < 6; ++index) {
                if (list[index] != nullptr) {
                    RawByte** result = (RawByte**) list[index];
                    list[index] = *((RawByte*) list[index]);
                    *result = &list[index];
                    
                    return (RawByte) ((RawByte)result + sizeof(RawByte**)); 
                }
            }

            throw std::bad_alloc();
        }

        void deallocate(void* pointer) {
            RawByte* node = (RawByte*) ((RawByte) pointer - sizeof(RawByte)); 
            RawByte** pointerToList = *((RawByte***)node);
            *node = (RawByte)*pointerToList;
            *pointerToList = node;
        } 

    private:
        size_t Rounder(const size_t size) {
            if (size <= minBlockSize - sizeof(RawByte)) {
                return minBlockSize;
            }

            size_t roundUp = minBlockSize;

            while (roundUp - sizeof(RawByte) < size) { 
                roundUp <<= 1; 
            }

            return roundUp;
        } 

        void BindBlocks(size_t size, size_t amount, unsigned int offset) {
            RawByte* constructor = (RawByte*) (allMemory + offset);

            for (size_t index = 1; index < amount; ++index) {
                *constructor = allMemory + offset + index * size;
                constructor = (RawByte*) (allMemory + offset + index * size);
            }

            *constructor = nullptr;
        } 
    RawByte list[6];
    RawByte allMemory;
    size_t  minBlockSize = 32;
};
