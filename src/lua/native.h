//
// Created by Jiří Zoudun on 22/11/15.
//

#ifndef LURUN_NATIVE_H
#define LURUN_NATIVE_H

#define LUA_NAT_PRINT 0
#define LUA_NAT_ASSERT 1
#define LUA_NAT_TOSTRING 2

namespace Lua {
    class Native {
        //Type of native function
        int type;

        // Helper methods
        const char * to_s(const ValueObject* vo);

    public:
        Native(int type) : type(type) {};

        void call(ValueObject**, int npar, int nres);

        void print();
    };
}


#endif //LURUN_NATIVE_H
