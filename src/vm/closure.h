
#ifndef LURUN_CLOSURE_H
#define LURUN_CLOSURE_H

namespace VM {

    class Closure {

    public:
        Lua::Function* function;

        Closure(Lua::Function* function);
        ~Closure();
    };

}

#endif //LURUN_CLOSURE_H
