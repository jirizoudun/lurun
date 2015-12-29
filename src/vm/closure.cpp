
#include "../common.h"

namespace VM {
    Closure::Closure(Function *function):
            proto(function)
    {}

    Closure::~Closure() {
        // DO NOT DELETE PROTO
    }
}