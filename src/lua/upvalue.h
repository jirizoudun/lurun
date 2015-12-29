//
// Created by Tomas on 16. 11. 2015.
//

#ifndef LURUN_UPVALUE_H
#define LURUN_UPVALUE_H

namespace Lua {

    /**
     * UpvalueDesc description
     */
    class UpvalueDesc {
    public:
        byte instack;
        byte idx;

        UpvalueDesc();
        UpvalueDesc(byte instack, byte idx);
        void print() const;
    };

    /**
     * Value of given upvalue. Only one per value in whole program.
     */
    struct UpvalueRef {

        UpvalueRef(ValueObject* ptr, UpvalueRef* next);
        UpvalueRef(ValueObject vo, UpvalueRef* next);

        /** either points to stack or to it's own ValueObject when closed */
        ValueObject* voPointer;

        /** ValueObject used when UpvalueRef is closed */
        ValueObject value;

        /** When open, link to next open UpvalueRef */
        UpvalueRef * next;
        UpvalueRef * prev;

        ValueObject getValue();

        void close();
    };
}

#endif //LURUN_UPVALUE_H
