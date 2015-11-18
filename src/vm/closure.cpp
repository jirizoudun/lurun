
#include "../common.h"

namespace VM {

    using namespace Lua;

    Closure::Closure(Function *function, int upvalueCount):
            function(function)
    {
        upvalues = new Upvalue[upvalueCount];
    }

    Closure::~Closure() {
        delete [] upvalues;
    }
}