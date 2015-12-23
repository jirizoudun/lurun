//
// Created by Jiří Zoudun on 22/11/15.
//

#ifndef LURUN_NATIVE_H
#define LURUN_NATIVE_H

#define LUA_NAT_PRINT 0

namespace Lua {
    class Native {
        //Static function defines
        static void printToConsole(const char *text);

        //Type of native function
        int type;

    public:
        Native(int type) : type(type) {};

        void call(ValueObject**, int npar, int nres);

        void print();
    };
}


#endif //LURUN_NATIVE_H
