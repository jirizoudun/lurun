
#include "../common.h"

namespace VM {
    Closure::Closure(Function *function): proto(function)
    {
        upvalues = new vector<UpvalueRef*>;
    }

    Closure::~Closure() {
        delete upvalues;
        // do not delete prototype here
    }
}