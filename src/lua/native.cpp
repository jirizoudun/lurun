
#include "../common.h"

namespace Lua {

    void Native::print() {
        printf("Native\n");
    }

    void Native::call(ValueObject** stack, int npar, int nres) {
        if (npar != 1 && nres != 0) { assert(false); }
        printf("%s\n", stack[1]->toString());
    }
}
