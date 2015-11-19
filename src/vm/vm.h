
#ifndef LURUN_VM_H
#define LURUN_VM_H

namespace VM {

    class VM {

        Lua::ValueObject stack[VM_STACK_SIZE];

        int stackTop;
        Lua::UpvalValue* openUpvals;
        Lua::UpvalValue* lastUpval;

        CallFrame* callStack[VM_CALL_STACK_SIZE]; // TODO better stack size, grow dynamically?
        int callStackTop;

    public:
        VM();

        void init(Lua::Function* initialChunk);
        void execute();
    };

}


#endif //LURUN_VM_H
