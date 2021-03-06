
#include "base_enviroment.h"

using namespace Lua;

namespace BaseEnv {
    void initBaseFunctions(Table *env) {
        env->set("_G",           ValueObject(LUA_TTABLE,  (void *)env));
        env->set("print",        ValueObject(LUA_TNATIVE, ALLOC_NATIVE(LUA_NAT_PRINT)));
        env->set("type",         ValueObject(LUA_TNATIVE, ALLOC_NATIVE(LUA_NAT_TYPE)));
        env->set("assert",       ValueObject(LUA_TNATIVE, ALLOC_NATIVE(LUA_NAT_ASSERT)));
        env->set("tonumber",     ValueObject(LUA_TNATIVE, ALLOC_NATIVE(LUA_NAT_TONUMBER)));
        env->set("tostring",     ValueObject(LUA_TNATIVE, ALLOC_NATIVE(LUA_NAT_TOSTRING)));
        env->set("next",         ValueObject(LUA_TNATIVE, ALLOC_NATIVE(LUA_NAT_NEXT)));
        env->set("pairs",        ValueObject(LUA_TNATIVE, ALLOC_NATIVE(LUA_NAT_PAIRS)));
        env->set("getmetatable", ValueObject(LUA_TNATIVE, ALLOC_NATIVE(LUA_NAT_GETMETATABLE)));
        env->set("setmetatable", ValueObject(LUA_TNATIVE, ALLOC_NATIVE(LUA_NAT_SETMETATABLE)));
    }

    void initIOTable(Table *env) {
        Table *io = (Table*)ALLOC_TABLE();
        io->set("write",         ValueObject(LUA_TNATIVE, ALLOC_NATIVE(LUA_NAT_IO_WRITE)));
        io->set("read",          ValueObject(LUA_TNATIVE, ALLOC_NATIVE(LUA_NAT_IO_READ)));
        io->set("open",          ValueObject(LUA_TNATIVE, ALLOC_NATIVE(LUA_NAT_IO_OPEN)));
        io->set("close",         ValueObject(LUA_TNATIVE, ALLOC_NATIVE(LUA_NAT_IO_CLOSE)));

        env->set("io", ValueObject(LUA_TTABLE, io));
    }

    void initMathTable(Table *env) {
        Table *math = (Table*)ALLOC_TABLE();
        math->set("ceil",         ValueObject(LUA_TNATIVE, ALLOC_NATIVE(LUA_NAT_MATH_CEIL)));

        env->set("math", ValueObject(LUA_TTABLE, math));
    }

    void initStringTable(Table *env) {
        Table *str = (Table*)ALLOC_TABLE();
        str->set("sub",         ValueObject(LUA_TNATIVE, ALLOC_NATIVE(LUA_NAT_STRING_SUB)));

        env->set("string",ValueObject(LUA_TTABLE, str));
    }


    void initEnviroment(Table *env) {
        initBaseFunctions(env);
        initIOTable(env);
        initMathTable(env);
        initStringTable(env);
    }
}