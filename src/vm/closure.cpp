
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

    void Closure::gc() const {
        for (std::vector<UpvalueRef*>::iterator it = upvalues->begin(); it != upvalues->end(); ++it) {
            HeapManager::markGray((char*)*it);
        }
    }
}