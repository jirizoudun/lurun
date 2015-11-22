#include "../common.h"

namespace VM {

    using namespace Lua;

    VM::VM() {
        stackTop = 0;
        callStackTop = 0;
        openUpvals = NULL;
        lastUpval = NULL;
    };

    void VM::init(Function *initialChunk) {
        printf("\nINIT\n\n");

        Table _ENV;

        initEnviroment(&_ENV);

        stack[stackTop++] = ValueObject(LUA_TTABLE, (void *) &_ENV);

        openUpvals = lastUpval = new UpvalueRef(stack);


        CallFrame *f = new CallFrame();
        f->closure = new ValueObject(LUA_TCLOSURE, new Closure(initialChunk));
        callStack[callStackTop++] = f;
    }

    void VM::execute() {
        printf("\nEXECUTE\n\n");
        ((Closure*)callStack[callStackTop - 1]->closure->value.p)->function->print();

    }

    void VM::initEnviroment(Table *env) {
        env->set("_G", ValueObject(LUA_TTABLE, (void *) env));
        env->print();

        env->set("print", new ValueObject(LUA_TNATIVE, (void *)(new Native(LUA_NAT_PRINT))));
        //TODO other native methods
    }

}
