//
// Created by Tomas on 15. 11. 2015.
//

#include "../common.h"

namespace Lua {

    ValueObject::~ValueObject() {
        switch(type) {
            case LUA_TSHRSTR:
            case LUA_TLNGSTR:
                delete ((String *)(value.p));
                break;
            default:
                break;
        }
    }

    void ValueObject::print() {
        switch(type) {
            case LUA_TNIL:
                printf("nil\n");
                break;
            case LUA_TBOOLEAN:
                printf("%s\n", (value.b ? "true" : "false"));
                break;
            case LUA_TNUMFLT:
                printf("%lf\n", value.d);
                break;
            case LUA_TNUMINT:
                printf("%i\n", value.i);
                break;
            case LUA_TSHRSTR:
            case LUA_TLNGSTR:
                ((String*)(value.p))->print();
                break;
            default:
                printf("???\n");
                break;
        }
    }
}
