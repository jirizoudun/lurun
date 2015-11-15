//
// Created by Tomas on 15. 11. 2015.
//

#ifndef LURUN_VALUE_H
#define LURUN_VALUE_H

#define LUA_TNIL        0
#define LUA_TBOOLEAN    1
#define LUA_TNUMFLT     3
#define LUA_TNUMINT     19
#define LUA_TSHRSTR     4
#define LUA_TLNGSTR     20

namespace Lua {

    // inspired by http://www.lua.org/doc/jucs05.pdf, page 5
    union Value {
    public:
        void*  p;
        int    i;
        double d;
        bool   b;
    };

    class ValueObject {
    public:
        int type;
        Value value;

        void print();
    };

}

#endif //LURUN_VALUE_H
