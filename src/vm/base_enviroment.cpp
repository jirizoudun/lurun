
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
        env->set("rawget",       ValueObject(LUA_TNATIVE, ALLOC_NATIVE(LUA_NAT_RAWGET)));
        env->set("rawset",       ValueObject(LUA_TNATIVE, ALLOC_NATIVE(LUA_NAT_RAWSET)));
        env->set("next",         ValueObject(LUA_TNATIVE, ALLOC_NATIVE(LUA_NAT_NEXT)));
        env->set("pairs",        ValueObject(LUA_TNATIVE, ALLOC_NATIVE(LUA_NAT_PAIRS)));
        env->set("getmetatable", ValueObject(LUA_TNATIVE, ALLOC_NATIVE(LUA_NAT_GETMETATABLE)));
        env->set("setmetatable", ValueObject(LUA_TNATIVE, ALLOC_NATIVE(LUA_NAT_SETMETATABLE)));
    }

    void initIOTable(Table *env) {
        Table *io = new Table();
        io->set("write",         ValueObject(LUA_TNATIVE, ALLOC_NATIVE(LUA_NAT_IO_WRITE)));
        io->set("open",          ValueObject(LUA_TNATIVE, ALLOC_NATIVE(LUA_NAT_IO_OPEN)));
        io->set("close",         ValueObject(LUA_TNATIVE, ALLOC_NATIVE(LUA_NAT_IO_CLOSE)));

        env->set("io", ValueObject(LUA_TTABLE, io));
    }

    void initEnviroment(Table *env) {
        initBaseFunctions(env);

        initIOTable(env);
    }
}