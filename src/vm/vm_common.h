
#ifndef LURUN_VM_COMMON_H
#define LURUN_VM_COMMON_H

#define VM_STACK_SIZE 250

#include "closure.h"
#include "call_frame.h"
#include "base_enviroment.h"
#include "vm.h"
#include "heap.h"


#define GC_TABLE   1
#define GC_UPVAL   2
#define GC_CLOSURE 3
#define GC_STRING  4
#define GC_NATIVE  5
#define GC_FILE    6

#define ALLOC_TABLE()            ((void*)(new (ALLOC_BLOCK(sizeof(Table),        GC_TABLE))    Table()))
#define ALLOC_UPVAL(STACK, NEXT) ((void*)(new (ALLOC_BLOCK(sizeof(UpvalueRef),   GC_UPVAL))    UpvalueRef(STACK, NEXT)))
#define ALLOC_CLOSURE(FUNCTION)  ((void*)(new (ALLOC_BLOCK(sizeof(Closure),      GC_CLOSURE))  Closure(FUNCTION)))
#define ALLOC_STRING(STR)        ((void*)(new (ALLOC_BLOCK(sizeof(StringObject), GC_STRING))   StringObject(STR)))
#define ALLOC_NATIVE(TYPE)       ((void*)(new (ALLOC_BLOCK(sizeof(Native),       GC_NATIVE))   Native(TYPE)))
#define ALLOC_FILE()             ((void*)(new (ALLOC_BLOCK(sizeof(File),         GC_FILE))     File()))
//#define ALLOC_FILE()             ((void*)(new File()))

#define ALLOC_BLOCK(SIZE, TYPE)  (::VM::HeapManager::allocBlock(SIZE, TYPE))

#endif //LURUN_VM_COMMON_H
