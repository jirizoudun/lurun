
#ifndef LURUN_CALL_FRAME_H
#define LURUN_CALL_FRAME_H

#include "../common.h"

namespace VM {

    struct CallFrame {

        Lua::ValueObject *closure; //FIXME Add closure into ValueObject
        Lua::ValueObject *top;
        Lua::ValueObject *base;
        CallFrame *prev;
    };

}

#endif //LURUN_CALL_FRAME_H
