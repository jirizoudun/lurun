
#ifndef LURUN_HEAP_H
#define LURUN_HEAP_H

#include "../common.h"

namespace VM {
using namespace Lua;

#define ALLOC_TABLE()            ((void*)(new Table()))
#define ALLOC_UPVAL(STACK, NEXT) ((void*)(new UpvalueRef(STACK, NEXT)))
#define ALLOC_CLOSURE(FUNCTION)  ((void*)(new Closure(FUNCTION)))
#define ALLOC_STRING(STR)        ((void*)(new StringObject(STR)))
#define ALLOC_NATIVE(TYPE)       ((void*)(new Native(TYPE)))

}

#endif //LURUN_HEAP_H
