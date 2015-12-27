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
}NativeType;

namespace Lua {
    class Native {
        //Type of native function
        NativeType type;

        // Helper methods
        char * to_s(const ValueObject* vo);

    public:
        Native(int type) : type((NativeType)type) {};

        void call(ValueObject**, int npar, int nres);

        void print();

        NativeType getType();
    };
}


#endif //LURUN_NATIVE_H