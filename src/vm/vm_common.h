
#ifndef LURUN_VM_COMMON_H
#define LURUN_VM_COMMON_H

#define VM_STACK_SIZE 250

#define HEAP_SIZE 268435456 // 256 MB
#define HEAP_HEAD_SIZE 2 // B
#define HEAP_MIN_BLOCK_SIZE 4
#define HEAP_MAX_BLOCK_SIZE 255

#define FULL_BLOCK_SIZE(BLOCK_SIZE) (BLOCK_SIZE + HEAP_HEAD_SIZE)

#define GC_TABLE   1
#define GC_UPVAL   2
#define GC_CLOSURE 3
#define GC_STRING  4
#define GC_NATIVE  5
#define GC_FILE    6

#define GC_WHITE 0
#define GC_BLACK 1
#define GC_GRAY  2
#define GC_PROTECTED 3

#include "closure.h"
#include "call_frame.h"
#include "base_enviroment.h"
#include "vm.h"
#include "heap.h"
#include "gc.h"

#define ALLOC_TABLE()            ((void*)(new (ALLOC_BLOCK(sizeof(Table),        GC_TABLE))    Table()))
#define ALLOC_UPVAL(STACK, NEXT) ((void*)(new (ALLOC_BLOCK(sizeof(UpvalueRef),   GC_UPVAL))    UpvalueRef(STACK, NEXT)))
#define ALLOC_CLOSURE(FUNCTION)  ((void*)(new (ALLOC_BLOCK(sizeof(Closure),      GC_CLOSURE))  Closure(FUNCTION)))
#define ALLOC_STRING(STR)        ((void*)(new (ALLOC_BLOCK(sizeof(StringObject), GC_STRING))   StringObject(STR)))
#define ALLOC_NATIVE(TYPE)       ((void*)(new (ALLOC_BLOCK(sizeof(Native),       GC_NATIVE))   Native(TYPE)))
#define ALLOC_FILE()             ((void*)(new (ALLOC_BLOCK(sizeof(File),         GC_FILE))     File()))

#define ALLOC_PROTECTED_STRING(STR) ((void*)(new (ALLOC_PROTECTED_BLOCK(sizeof(StringObject), GC_STRING))   StringObject(STR)))

#define ALLOC_BLOCK(SIZE, TYPE)           (::VM::HeapManager::allocBlock(SIZE, TYPE, false))
#define ALLOC_PROTECTED_BLOCK(SIZE, TYPE) (::VM::HeapManager::allocBlock(SIZE, TYPE, true))

#endif //LURUN_VM_COMMON_H
