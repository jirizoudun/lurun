
#ifndef LURUN_CALL_FRAME_H
#define LURUN_CALL_FRAME_H

#include "../common.h"

namespace VM {

    struct CallFrame {
        Closure *closure;
        int top; // first free
        int base;
        int size;
        CallFrame *prev;

        int npar;
        int nres;

        int stack_max;

        CallFrame(CallFrame* prev, Closure* closure, int base, int top, int npar=0, int nres=0);
        ~CallFrame();
    };
}

#endif //LURUN_CALL_FRAME_H
