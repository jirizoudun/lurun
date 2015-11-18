
#ifndef LURUN_CLOSURE_H
#define LURUN_CLOSURE_H

namespace VM {

    class Closure {

        Lua::Function* function;
        Lua::Upvalue* upvalues;

    public:
        Closure(Lua::Function* function, int upvalueCount);
        ~Closure();
    };

}

#endif //LURUN_CLOSURE_H
