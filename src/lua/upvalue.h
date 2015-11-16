//
// Created by Tomas on 16. 11. 2015.
//

#ifndef LURUN_UPVALUE_H
#define LURUN_UPVALUE_H

namespace Lua {

    class Upvalue {
    public:
        byte instack;
        byte idx;

        Upvalue();
        Upvalue(byte instack, byte idx);
        void print() const;
    };
}

#endif //LURUN_UPVALUE_H
