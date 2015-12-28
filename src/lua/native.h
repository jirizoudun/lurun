//
// Created by Jiří Zoudun on 22/11/15.
//

#ifndef LURUN_NATIVE_H
#define LURUN_NATIVE_H

typedef enum NATIVE_TYPE {
    LUA_NAT_PRINT = 0,
    LUA_NAT_ASSERT,
    LUA_NAT_TOSTRING,
    LUA_NAT_TONUMBER,
    LUA_NAT_RAWGET,
    LUA_NAT_RAWSET,
    LUA_NAT_NEXT,
    LUA_NAT_PAIRS,

    LUA_NAT_IO_OPEN = 100,
    LUA_NAT_IO_CLOSE,
    LUA_NAT_IO_WRITE
}NativeType;

namespace Lua {
    class Native {
        //Type of native function
        NativeType type;

        // Helper methods
        char * to_s(const ValueObject* vo);

        Table* env;

    public:
        Native(int type, Table* env) : type((NativeType)type), env(env) {};

        void call(ValueObject**, int npar, ValueObject** base_res, int nres);

        void print();

        NativeType getType();
    };
}


#endif //LURUN_NATIVE_H
