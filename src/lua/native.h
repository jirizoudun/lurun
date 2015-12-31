
#ifndef LURUN_NATIVE_H
#define LURUN_NATIVE_H

typedef enum NATIVE_TYPE {
    LUA_NAT_PRINT = 0,
    LUA_NAT_TYPE,
    LUA_NAT_ASSERT,
    LUA_NAT_TOSTRING,
    LUA_NAT_TONUMBER,
    LUA_NAT_RAWGET,
    LUA_NAT_RAWSET,
    LUA_NAT_NEXT,
    LUA_NAT_PAIRS,
    LUA_NAT_GETMETATABLE,
    LUA_NAT_SETMETATABLE,

    LUA_NAT_IO_OPEN = 100,
    LUA_NAT_IO_CLOSE,
    LUA_NAT_IO_WRITE,
    LUA_NAT_IO_READ,

    LUA_NAT_FILE_WRITE = 200,
    LUA_NAT_FILE_READ,

    LUA_NAT_MATH_CEIL = 300,

    LUA_NAT_STRING_SUB = 400
}NativeType;

namespace Lua {
    class Native {
        NativeType type;

    public:
        Native(int type) : type((NativeType)type) {};

        int call(ValueObject*, int npar, ValueObject* base_res, int nres);

        void print();

        NativeType getType();
    };
}


#endif //LURUN_NATIVE_H
