//
// Created by Jiří Zoudun on 22/11/15.
//

#ifndef LURUN_NATIVE_H
#define LURUN_NATIVE_H

enum lua_natives {
    LUA_NAT_PRINT = 0,
    LUA_NAT_ASSERT,
    LUA_NAT_TOSTRING,
    LUA_NAT_TONUMBER,
    LUA_NAT_RAWGET,
    LUA_NAT_RAWSET,
};

namespace Lua {
    class Native {
        //Type of native function
        int type;

        // Helper methods
        char * to_s(const ValueObject* vo);

    public:
        Native(int type) : type(type) {};

        void call(ValueObject**, int npar, int nres);

        void print();
    };
}


#endif //LURUN_NATIVE_H
