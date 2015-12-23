
#include "../common.h"

namespace VM {

    CallFrame::CallFrame(CallFrame* prev, Closure* closure, int top, int base):
            prev(prev), closure(closure), top(top), base(base)
    {}

    CallFrame::~CallFrame() {
        // TODO
    }
}
