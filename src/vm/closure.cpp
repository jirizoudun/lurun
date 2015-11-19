
#include "../common.h"

namespace VM {

    using namespace Lua;

    Closure::Closure(Function *function):
            function(function)
    {
    }

    Closure::~Closure() {
    }
}