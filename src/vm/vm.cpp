
#include "../common.h"

namespace VM {

    using namespace Lua;

    VM::VM() {
        stackTop = 0;
        callStackTop = 0;
        openUpvals = NULL;
        lastUpval = NULL;
    };

    void VM::init(Function* initialChunk) {
        Table _ENV;

        _ENV.set("_G", ValueObject(LUA_TTABLE, (void*)&_ENV));
        _ENV.print();

        stack[stackTop++] = ValueObject(LUA_TTABLE, (void*)&_ENV);

        openUpvals = lastUpval = new UpvalValue(stack);


        CallFrame* f = new CallFrame();
        f->cl = new Closure(initialChunk);
        callStack[callStackTop++] = f;
    }

    void VM::execute() {

        printf("\nEXECUTE\n\n");
        callStack[callStackTop-1]->cl->function->print();

    }

}
