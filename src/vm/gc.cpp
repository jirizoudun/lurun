
#include "../common.h"

namespace VM {
    using namespace Lua;

    /** Create root set from stack */
    void GC::root(ValueObject *stack, CallFrame *frame) {
        for (int i = 0; i <= frame->stack_max; i++) {
            stack[i].gc();
        }

        // better safe than sorry, add all closures from upper call frames
        do {
            HeapManager::markGray((char *) frame->closure);
            frame = frame->prev;
        } while (frame != NULL);
    };

    void GC::mark() {
        while (!HeapManager::gray.empty()) { // TODO shouldn't do all at once
            HeapManager::markReferencesGray();
        }

        /*
        if (HeapManager::gray.empty()) {
            sweep();
        }*/
    }

    void GC::sweep(bool force) {
        HeapManager::purgeHeap(force);

#if DEBUG_HEAP
        HeapManager::printStatus();
#endif
    }

}
