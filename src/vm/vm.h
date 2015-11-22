
#ifndef LURUN_VM_H
#define LURUN_VM_H

#include "../lua/table.h"

namespace VM {

    class VM {

        Lua::ValueObject stack[VM_STACK_SIZE];

        int stackTop;
        Lua::UpvalueRef * openUpvals;
        Lua::UpvalueRef * lastUpval;

        CallFrame* callStack[VM_CALL_STACK_SIZE]; // TODO better stack size, grow dynamically?
        int callStackTop;

        void initEnviroment(Lua::Table *env);

    public:
        VM();

        void init(Lua::Function* initialChunk);
        void execute();
    };

}


#endif //LURUN_VM_H
