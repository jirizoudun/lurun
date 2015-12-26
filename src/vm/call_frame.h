
#ifndef LURUN_CALL_FRAME_H
#define LURUN_CALL_FRAME_H

#include "../common.h"

namespace VM {

    struct CallFrame {
        Closure *closure;
        int top;
        int base;
        CallFrame *prev;

        int npar;
        int nres;

        CallFrame(CallFrame* prev, Closure* closure, int top, int base, int npar=0, int nres=0);
        ~CallFrame();
    };
}

#endif //LURUN_CALL_FRAME_H
