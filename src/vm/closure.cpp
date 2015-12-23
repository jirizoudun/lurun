
#include "../common.h"

namespace VM {
    Closure::Closure(Function *function):
            proto(function)
    {}

    Closure::~Closure() {
    }

    void Closure::print() {
        printf("Closure\n");
    }
}