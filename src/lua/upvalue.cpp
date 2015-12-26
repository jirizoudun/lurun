//
// Created by Tomas on 16. 11. 2015.
//

#include "../common.h"

namespace Lua {

    UpvalueDesc::UpvalueDesc() {}
    UpvalueDesc::UpvalueDesc(byte instack, byte idx):
            instack(instack),

            idx(idx)
    {}

    void UpvalueDesc::print() const {
        printf("In stack: %i, idx: %i\n", instack, idx);
    }

    UpvalueRef::UpvalueRef(ValueObject* ptr) {
        voPointer = ptr;
        next = NULL;
    }
    UpvalueRef::UpvalueRef(ValueObject vo):
            value(vo)
    {
        next = NULL;
    }

    ValueObject* UpvalueRef::getValue() {
        if (voPointer != NULL) {
            return voPointer;
        } else {
            return &value;
        }
    }

}