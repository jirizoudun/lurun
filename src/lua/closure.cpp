
#include "../common.h"

namespace Lua {
    Closure::Closure(Function *function):
            function(function)
    {
    }

    Closure::~Closure() {
    }

    void Closure::print() {
        printf("Closure\n");
    }
}