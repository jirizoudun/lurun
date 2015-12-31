

#include "../common.h"

namespace VM {
    using namespace Lua;

    char HeapManager::heap[HEAP_SIZE];
    char* HeapManager::next_free_block = heap;

    set<char*> HeapManager::gray;

    void HeapManager::read_block_size(char* ptr, unsigned char& block_size) {
        block_size = (unsigned char)*ptr;
    }
    void HeapManager::read_block_flags(char* ptr, char& type, char& color, bool& free) {
        char flags = *(ptr+1);
        type =  (flags & (char)0b11111000) >> 3;
        color = (flags & (char)0b00000110) >> 1;
        free = !(flags & (char)0b00000001);
    }

    void HeapManager::write_block_size(char* ptr, unsigned char block_size) {
        *ptr = block_size;
    }
    void HeapManager::write_block_flags(char* ptr, char type, char color, bool free) {
        char flags = 0;
        flags |= ((type  << 3) & 0b11111000);
        flags |= ((color << 1) & 0b00000110);
        flags |= (!free        & 0b00000001);
        *(ptr+1) = flags;
    }

    char* HeapManager::getAddress(unsigned char alloc_size, unsigned char& block_size) {
        char type,color;
        bool free;

        int p = (int)(next_free_block - heap);
        while (p < HEAP_SIZE - HEAP_HEAD_SIZE - alloc_size) {
            read_block_size(heap+p, block_size);
            read_block_flags(heap+p, type, color, free);

            if (free && (block_size == 0 || alloc_size <= block_size)) { return heap+p; }
            p += FULL_BLOCK_SIZE(block_size);
        }
        return NULL;
    }

    void* HeapManager::allocBlock(unsigned char alloc_size, char type, bool protect) {
        assert(alloc_size <= 255);
        unsigned char block_size;
        char* ptr = getAddress(alloc_size, block_size);

        if (ptr == NULL) {
            printf("We ran out of memory\n");
            exit(1);
        }

#if DEBUG_HEAP_ALLOC
        printf("ALLOC %i %i\n", (int)(ptr - heap), type);
#endif

        int next_free_block_size = 0;

        // I'm not at the end of heap, there is block after me
        if (block_size != 0) {
            next_free_block_size = block_size - alloc_size - HEAP_HEAD_SIZE;

            // if new free block would be smaller than minimum,
            // use whole block as is and do nothing, since we
            // reached other set-up block
            if (next_free_block_size < HEAP_MIN_BLOCK_SIZE) {
                alloc_size = block_size;
            // else create new block with new size
            } else {
                write_block_size(ptr + FULL_BLOCK_SIZE(alloc_size), (unsigned char)next_free_block_size);
                write_block_flags(ptr + FULL_BLOCK_SIZE(alloc_size), 0, 0, 1);
            }
        } else {
            write_block_size(ptr + FULL_BLOCK_SIZE(alloc_size), 0);
            write_block_flags(ptr + FULL_BLOCK_SIZE(alloc_size), 0, 0, 1);
        }

        write_block_size(ptr, alloc_size);
        write_block_flags(ptr, type, (char)(protect ? GC_PROTECTED : GC_WHITE), false);

        if (next_free_block == ptr) {
            next_free_block += FULL_BLOCK_SIZE(alloc_size);
        }

        return ptr + HEAP_HEAD_SIZE;
    }

    void HeapManager::purgeHeap(bool force) {

        unsigned char block_size;
        char type, color;
        bool free;

        next_free_block = NULL;
        char* prev_block = NULL;
        unsigned char prev_size = 0;

        int p = 0;
        while (p < HEAP_SIZE - HEAP_HEAD_SIZE - HEAP_MIN_BLOCK_SIZE) {
            char* block = heap+p;

            read_block_size(block, block_size);

            if (block_size == 0) { // reached the end of used space
                if (next_free_block == NULL) { next_free_block = block; }
                if (prev_block != NULL) { write_block_size(prev_block, 0); }
                break;
            }

            read_block_flags(block, type, color, free);

            // free white blocks
            if (!free && (color == GC_WHITE || force)) {

#if DEBUG_HEAP_ALLOC
                printf("DEALLOC %i\n", (int)(block - heap));
#endif

                switch(type) {
                    case GC_TABLE:   ((Table*)       (block + HEAP_HEAD_SIZE))->~Table();        break;
                    case GC_UPVAL:   ((UpvalueRef*)  (block + HEAP_HEAD_SIZE))->~UpvalueRef();   break;
                    case GC_CLOSURE: ((Closure*)     (block + HEAP_HEAD_SIZE))->~Closure();      break;
                    case GC_STRING:  ((StringObject*)(block + HEAP_HEAD_SIZE))->~StringObject(); break;
                    case GC_NATIVE:  ((Native*)      (block + HEAP_HEAD_SIZE))->~Native();       break;
                    case GC_FILE:    ((File*)        (block + HEAP_HEAD_SIZE))->~File();         break;
                    default: break;
                }
                free = true;
            }

            if (!free) {
                write_block_flags(block, type, (char)(color == GC_PROTECTED ? GC_PROTECTED : GC_WHITE), free);
                prev_block = NULL;
            } else {
                if (next_free_block == NULL) { next_free_block = block; }

                // if possible, merge with previous free block
                if (prev_block != NULL && prev_size + block_size + HEAP_HEAD_SIZE <= HEAP_MAX_BLOCK_SIZE) {
                    write_block_size(prev_block, (unsigned char)(prev_size + block_size + HEAP_HEAD_SIZE));
                    prev_size = (unsigned char)(prev_size + block_size + HEAP_HEAD_SIZE);
                // otherwise just mark block as free
                } else {
                    write_block_flags(block, type, GC_WHITE, free);
                    prev_block = block;
                    prev_size = block_size;
                }
            }

            p += FULL_BLOCK_SIZE(block_size);
        }
    }

    void HeapManager::markGray(char* ptr) {
        if (gray.find(ptr) == gray.end()) {
            char dummy_type;
            char color;
            bool free;

            char* block_ptr = ptr - HEAP_HEAD_SIZE;

            // if not gray check if white
            read_block_flags(block_ptr, dummy_type, color, free);

            //printf("INDEX = %i\n", (ptr - HeapManager::heap - 2));
            //printf("TYPE = %i\n", dummy_type);

            assert(!free); // sanity check
            if (color == GC_WHITE) {
                write_block_flags(block_ptr, dummy_type, GC_GRAY, free);
                gray.insert(ptr);
            }
        }
    }

    void HeapManager::markReferencesGray() {
        char type, dummy_color;
        bool dummy_free;

        for (std::set<char*>::iterator ptr = gray.begin(); ptr != gray.end();) {
            char* block_ptr = (*ptr) - HEAP_HEAD_SIZE;
            read_block_flags(block_ptr, type, dummy_color, dummy_free);

            ((GCObject*)*ptr)->gc(); // mark all references from the object gray
            write_block_flags(block_ptr, type, GC_BLACK, dummy_free);

            ptr = gray.erase(ptr);
        }
    }

    void HeapManager::print() {

        unsigned char block_size;
        char type;
        char color;
        bool free;
        int p = 0;

        printf("Skipping protected\n");
        while(p < HEAP_SIZE - HEAP_HEAD_SIZE) {
            read_block_size(heap+p, block_size);
            read_block_flags(heap+p, type, color, free);

            if (color != GC_PROTECTED) {
                if (block_size == 0) { // reached free space at the and of the heap
                    double left = (HEAP_SIZE - p)/1024.;
                    printf("%.3f kB (%.3f MB) left\n\n", left, left/1024.);
                    break;
                }

                switch(color) {
                    case GC_WHITE: printf("W"); break;
                    case GC_BLACK: printf("B"); break;
                    case GC_GRAY:  printf("G"); break;
                    case GC_PROTECTED:  printf("*"); break;
                    default: break;
                }

                printf(" Block [%i] @%i: %iB", free, p, block_size);
                if (!free) {
                    switch(type) {
                        case GC_TABLE:   printf(", table"); break;
                        case GC_UPVAL:   printf(", upval"); break;
                        case GC_CLOSURE: printf(", closure"); break;
                        case GC_STRING:  printf(", string"); break;
                        case GC_NATIVE:  printf(", native"); break;
                        case GC_FILE:    printf(", file"); break;
                        default: printf("unknown"); break;
                    }
                }
                printf("\n");
            }

            p += HEAP_HEAD_SIZE + block_size;
        }
    }

    void HeapManager::printStatus() {

        int free_blocks = 0, used_blocks = 0;
        int free_memory = 0, used_memory = 0;

        unsigned char block_size;
        char type, color;
        bool free;
        int p = 0;
        while(p < HEAP_SIZE - HEAP_HEAD_SIZE) {
            read_block_size(heap+p, block_size);
            read_block_flags(heap+p, type, color, free);

            if (block_size == 0) { // reached free space at the and of the heap
                printf("Free: %.3f kB (%i blocks), used: %.3f kB (%i blocks)\n",
                       free_memory/1024., free_blocks,
                       used_memory/1024., used_blocks);
                double left = (HEAP_SIZE - p)/1024.;
                printf("%.3f kB (%.3f MB) left\n\n", left, left/1024.);
                break;
            }

            if (free) {
                free_blocks++;
                free_memory += block_size;
            } else {
                used_blocks++;
                used_memory += block_size;
            }

            p += HEAP_HEAD_SIZE + block_size;
        }
    }

}
