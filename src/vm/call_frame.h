
#ifndef LURUN_CALL_FRAME_H
#define LURUN_CALL_FRAME_H

#include "../common.h"

namespace VM {

    struct CallFrame {
        Closure *closure;
        int top;
        int base;
        CallFrame *prev;

        CallFrame(CallFrame* prev, Closure* closure, int top, int base);
        ~CallFrame();
    };
}

#endif //LURUN_CALL_FRAME_H
