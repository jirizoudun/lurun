//
// Created by Tomas on 15. 11. 2015.
//

#ifndef LURUN_VALUE_H
#define LURUN_VALUE_H

#include "string.h"

#define LUA_TNIL        0
#define LUA_TBOOLEAN    1
#define LUA_TNUMFLT     3
#define LUA_TNUMINT     19
#define LUA_TSHRSTR     4
#define LUA_TLNGSTR     20
#define LUA_TTABLE		5
#define LUA_TCLOSURE    6
#define LUA_TNATIVE     99

namespace Lua {

    // inspired by http://www.lua.org/doc/jucs05.pdf, page 5
    union Value {
    public:
        void*       p;
        long long   i;
        double      d;
        bool        b;
    };

    class ValueObject {
    public:
        int type;
        Value value;

        ValueObject();
        ValueObject(bool b);
        ValueObject(double d);
        ValueObject(int i);
        ValueObject(long long i);
        ValueObject(int type, void* ptr);

        ValueObject(const ValueObject&);
        //ValueObject& operator= (const ValueObject& other);

        ~ValueObject();
        void print() const;

        bool operator==(const ValueObject& other) const;
    };

    struct ValueObjectHasher {
        size_t operator()(const ValueObject& o) const {
            switch (o.type) {
                case LUA_TBOOLEAN:
                    return std::hash<bool>()(o.value.b);
                case LUA_TNUMFLT:
                    return std::hash<double>()(o.value.d);
                case LUA_TNUMINT:
                    return std::hash<long long>()(o.value.i);
                case LUA_TSHRSTR:
                case LUA_TLNGSTR:
                    return ((String *)(o.value.p))->getHash();
                default:
                case LUA_TTABLE:
                case LUA_TCLOSURE:
                case LUA_TNATIVE:
                    // NOT SUPPORTED
                    assert(false);
            }
        }
    };
}

#endif //LURUN_VALUE_H
