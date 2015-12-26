
#include "../common.h"

namespace VM {

    CallFrame::CallFrame(CallFrame* prev, Closure* closure, int top, int base, int npar, int nres):
            prev(prev), closure(closure), top(top), base(base), npar(npar), nres(nres)
    {}

    CallFrame::~CallFrame() {
        // TODO
    }
}
