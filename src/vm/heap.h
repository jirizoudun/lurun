
#ifndef LURUN_HEAP_H
#define LURUN_HEAP_H

#include "../common.h"

namespace VM {

using namespace Lua;

    class HeapManager {

        static char heap[HEAP_SIZE];
        static int next_free_block;

        static void read_block_head(char* ptr, unsigned char& block_size, char& type, bool& free);
        static void write_block_head(char* ptr, unsigned char block_size, char type);

        /** Get address of next free block (including header) on heap */
        static void* getAddress(size_t size);

    public:

        /** Set flags for allocation and return address of block contents */
        static void* allocBlock(size_t size, char type);

        static void print();
    };
}

#endif //LURUN_HEAP_H
