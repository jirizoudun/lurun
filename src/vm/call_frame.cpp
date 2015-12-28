
#include "../common.h"

namespace VM {

    CallFrame::CallFrame(CallFrame* prev, Closure* closure, int base, int top, int npar, int nres):
            prev(prev), closure(closure), base(base), top(top), npar(npar), nres(nres)
    {
        size = closure->proto->getMaxStackSize();
    }

    CallFrame::~CallFrame() {
        // TODO
    }
}
