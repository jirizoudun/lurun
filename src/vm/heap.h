
#ifndef LURUN_HEAP_H
#define LURUN_HEAP_H

#include "../common.h"

namespace VM {

using namespace Lua;

    class HeapManager {


        static char* next_free_block;

        static void read_block_size(char* ptr, unsigned char& block_size);
        static void read_block_flags(char* ptr, char& type, char& color, bool& free);

        static void write_block_size(char* ptr, unsigned char block_size);
        static void write_block_flags(char* ptr, char type, char color, bool free);

        /** Get address of next free block (including header) on heap */
        static char* getAddress(unsigned char alloc_size, unsigned char& block_size);

    public:

        static char heap[HEAP_SIZE];
        static std::set<char*> gray;

        /** Set flags for allocation and return address of block contents */
        static void* allocBlock(unsigned char size, char type, bool protect);
        static void purgeHeap(bool force);

        static void markGray(char* ptr);
        static void markReferencesGray();

        static void print();
        static void printStatus();
    };
}

#endif //LURUN_HEAP_H
