//
// Created by Tomas on 16. 11. 2015.
//

#ifndef LURUN_UPVALUE_H
#define LURUN_UPVALUE_H

namespace Lua {

    /**
     * Upvalue description
     */
    class Upvalue {
    public:
        byte instack;
        byte idx;

        Upvalue();
        Upvalue(byte instack, byte idx);
        void print() const;
    };

    /**
     * Value of given upvalue. Only one per value in whole program.
     */
    struct UpvalValue {

        UpvalValue(ValueObject* ptr);
        UpvalValue(ValueObject vo);

        /** either points to stack or to it's own ValueObject when closed */
        ValueObject* voPointer;

        /** ValueObject used when Upvalue is closed */
        ValueObject value;

        /** When open, link to next open Upvalue */
        UpvalValue* next;
    };
}

#endif //LURUN_UPVALUE_H
