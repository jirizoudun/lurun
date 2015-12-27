//
// Created by Jiří Zoudun on 27/12/15.
//

#include "base_enviroment.h"

using namespace Lua;

namespace BaseEnv {
    void initBaseFunctions(Table *env) {
        env->set("_G", ValueObject(LUA_TTABLE, (void *) env));
        env->set("print", ValueObject(LUA_TNATIVE, (void *) (new Native(LUA_NAT_PRINT))));
        env->set("assert", ValueObject(LUA_TNATIVE, (void *) (new Native(LUA_NAT_ASSERT))));
        env->set("tonumber", ValueObject(LUA_TNATIVE, (void *) (new Native(LUA_NAT_TONUMBER))));
        env->set("tostring", ValueObject(LUA_TNATIVE, (void *) (new Native(LUA_NAT_TOSTRING))));
        env->set("rawget", ValueObject(LUA_TNATIVE, (void *) (new Native(LUA_NAT_RAWGET))));
        env->set("rawset", ValueObject(LUA_TNATIVE, (void *) (new Native(LUA_NAT_RAWSET))));
    }

    void initIOTable(Table *env) {
        Table *io = new Table();
        io->set("write", ValueObject(LUA_TNATIVE, (void *) (new Native(LUA_NAT_IO_WRITE))));
//        io->set("open", ValueObject(LUA_TNATIVE, (void *) (new Native(LUA_NAT_IO_OPEN))));
    }

    void initEnviroment(Table *env) {
        initBaseFunctions(env);

        initIOTable(env);
    }
}