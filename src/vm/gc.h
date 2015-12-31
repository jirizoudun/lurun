
#ifndef LURUN_GC_H
#define LURUN_GC_H

namespace VM {
    using namespace Lua;

    class GC {
    public:

        /** Create root set from stack */
        static void root(ValueObject* stack, int top, CallFrame* frame) {
            for (int i = 0; i < frame->base + frame->size; i++) {
                stack[i].gc();
            }

            // better safe than sorry, add all closures from upper call frames
            do {
                HeapManager::markGray((char*)frame->closure);
                frame = frame->prev;
            } while(frame != NULL);
        };

        static void mark() {
            while(!HeapManager::gray.empty()) { // TODO shouldn't do all at once
                HeapManager::markReferencesGray();
            }

            if (HeapManager::gray.empty()) {
                sweep();
            }
        }

        static void sweep() {
            HeapManager::purgeHeap();
#if DEBUG_HEAP
            HeapManager::printStatus();
#endif
        }
    };
}



#endif //LURUN_GC_H
