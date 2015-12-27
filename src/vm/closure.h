
#ifndef LURUN_CLOSURE_H
#define LURUN_CLOSURE_H

namespace VM {

    using namespace Lua;

    class Closure {

    public:
        Function* proto;
        std::vector<UpvalueRef*> upvalues;

        Closure(Function* function);
        ~Closure();
    };

}

#endif //LURUN_CLOSURE_H
