
#ifndef LURUN_VM_H
#define LURUN_VM_H

#include "../lua/table.h"

namespace VM {

    class VM {

        Lua::ValueObject* stack[VM_STACK_SIZE];

        int stackTop;
        Lua::UpvalueRef * openUpvals;
        Lua::UpvalueRef * lastUpval;

        CallFrame* topCallFrame;

        void initEnviroment(Lua::Table *env);
        void execute(CallFrame*);
    public:
        VM();

        void init(Lua::Function* initialChunk);
        void run();
        void printStack() const;
    };

}


#endif //LURUN_VM_H
