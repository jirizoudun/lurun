
#ifndef LURUN_VM_H
#define LURUN_VM_H

#include "../lua/table.h"

namespace VM {

    class VM {

        Lua::ValueObject* stack[VM_STACK_SIZE];

        Lua::UpvalueRef * lastUpval;

        CallFrame* topCallFrame;

        void initEnviroment(Lua::Table *env);
        void execute(CallFrame*);

        ValueObject* arithmetic(long long, long long, OpCode);
        ValueObject* arithmetic(double, double, OpCode);

        template <class T, class U>
        bool cmp(T, U, OpCode);

        ValueObject getVO(ValueObject** stack, Function * proto, int R) const;

        bool callM(CallFrame* ci, ValueObject B, ValueObject C, int RA, const char*);
    public:
        VM();

        void init(Lua::Function* initialChunk);
        void run();
        void printStack(CallFrame*) const;
    };

}


#endif //LURUN_VM_H
