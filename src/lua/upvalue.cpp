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

    UpvalueRef::UpvalueRef(ValueObject* ptr, UpvalueRef* next) {
        voPointer = ptr;
        this->next = next;
        prev = NULL;
        if (next != NULL) {
            next->prev = this;
        }
    }
    UpvalueRef::UpvalueRef(ValueObject vo, UpvalueRef* next):
            value(vo), next(next)
    {
        prev = NULL;
        next->prev = this;
    }

    ValueObject UpvalueRef::getValue() {
        // TODO check usage
        if (voPointer != NULL) {
            return *voPointer;
        } else {
            return value;
        }
    }

    void UpvalueRef::close() {
        if(voPointer == NULL) {return;}

        value = *voPointer;
        voPointer = NULL;

        UpvalueRef* p = prev;
        UpvalueRef* n = next;

        if (prev != NULL) {prev->next = next;}
        if (next != NULL) {next->prev = prev;}

    }

}