
#include "../common.h"

namespace VM {
    using namespace Lua;

    int HeapManager::next_free_block = 0;
    char HeapManager::heap[HEAP_SIZE];

    void HeapManager::read_block_head(char* ptr, char& block_size, char& type, bool& free) {
        block_size = ptr[0];
        char flags = ptr[1];
        type =  (flags & (char)0b11111000) >> 3;
        free = !(flags & (char)1);
    }
    void HeapManager::write_block_head(char* ptr, char block_size, char type) {
        char flags = 0;
        flags |= (type << 3);
        flags |= (char)1;

        ptr[0] = block_size;
        ptr[1] = flags;
    }

    void* HeapManager::getAddress(size_t alloc_size) {
        char block_size;
        char type;
        bool free;

        int p = next_free_block;
        while (p < HEAP_SIZE - HEAP_HEAD_SIZE - alloc_size) {
            read_block_head(heap+p, block_size, type, free);

            if (free && (block_size == 0 || alloc_size <= block_size)) { return heap+p; }
            p += HEAP_HEAD_SIZE + block_size;
        }

        return NULL;
    }

    void* HeapManager::allocBlock(size_t alloc_size, char type) {
        assert(alloc_size <= 255);
        void* ptr = getAddress(alloc_size);

        if (ptr == NULL) {
            printf("We ran out of memory\n");
            exit(1);
        }

        write_block_head((char*)ptr, (char)alloc_size, type);

        // TODO handle case when block is not last free block
        // TODO set next free block

        return (char*)ptr + HEAP_HEAD_SIZE;
    }

    void HeapManager::print() {

        char block_size;
        char type;
        bool free;
        int p = 0;
        while(p < HEAP_SIZE - HEAP_HEAD_SIZE) {
            read_block_head(heap+p, block_size, type, free);
            if (block_size == 0) {break;} // reached free space at the and of the heap

            printf("Block [%i] @%i: %iB", free, p, block_size);
            if (!free) {
                switch(type) {
                    case GC_TABLE:   printf(", table"); break;
                    case GC_UPVAL:   printf(", upval"); break;
                    case GC_CLOSURE: printf(", closure"); break;
                    case GC_STRING:  printf(", string"); break;
                    case GC_NATIVE:  printf(", native"); break;
                    default: printf("unknown"); break;
                }
            }
            printf("\n");

            p += HEAP_HEAD_SIZE + block_size;
        }
    }
}
