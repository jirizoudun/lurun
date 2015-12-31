
#ifndef LURUN_CLOSURE_H
#define LURUN_CLOSURE_H

namespace VM {

    using namespace Lua;

    class Closure: public GCObject {

    public:
        Function* proto;
        std::vector<UpvalueRef*>* upvalues;

        Closure(Function* function);
        ~Closure();

        void gc() const;
    };

}

#endif //LURUN_CLOSURE_H
