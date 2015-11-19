//
// Created by Tomas on 16. 11. 2015.
//

#include "../common.h"

namespace Lua {

    Upvalue::Upvalue() {}
    Upvalue::Upvalue(byte instack, byte idx):
            instack(instack),

            idx(idx)
    {}

    void Upvalue::print() const {
        printf("In stack: %i, idx: %i\n", instack, idx);
    }

    UpvalValue::UpvalValue(ValueObject* ptr):
            voPointer(ptr)
    {
        next = NULL;
    }
    UpvalValue::UpvalValue(ValueObject vo):
            value(vo)
    {
        next = NULL;
    }

}