
#ifndef LURUN_GC_H
#define LURUN_GC_H

namespace VM {
    using namespace Lua;

    class GC {
    public:

        /** Creates root set from stack */
        static void root(ValueObject* stack, CallFrame* frame);
        static void mark();
        static void sweep(bool force=false);
    };
}

#endif //LURUN_GC_H
