
#include "../common.h"

namespace VM {

    CallFrame::CallFrame(CallFrame* prev, Closure* closure, int base, int top, int npar, int nres):
            prev(prev), closure(closure), base(base), top(top), npar(npar), nres(nres)
    {
        size = closure->proto->getMaxStackSize();

        if (prev == NULL || base + size > prev->stack_max) {
            stack_max = base + size;
        } else {
            stack_max = prev->stack_max;
        }
    }

    CallFrame::~CallFrame() {
        // do nothing, closure should be GC'd
    }
}
