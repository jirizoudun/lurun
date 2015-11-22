
#ifndef LURUN_CLOSURE_H
#define LURUN_CLOSURE_H

namespace Lua {

    using namespace Lua;

    class Closure {

    public:
        Function* function; //< prototype
        Container<UpvalueRef>* upvalues;

        Closure(Function* function);
        ~Closure();

        void print();
    };

}

#endif //LURUN_CLOSURE_H
