//
// Created by Tomas on 15. 11. 2015.
//

#ifndef LURUN_INSTRUCTION_H
#define LURUN_INSTRUCTION_H

#include "opcodes.h"

#define ARGS_COUNT 3

namespace Lua {

    typedef enum {
        ABC = 0,
        ABX,
        ASBX,
        AX,
    }InstructionType;

    class Instruction {

        OpCode instruction;
        int args[ARGS_COUNT];
        InstructionType type;

        void decode(int instruction);

    public:
        Instruction(int inst);
        void print();

        OpCode getOpCode();
        int* getArgs();
    };
}


#endif //LURUN_INSTRUCTION_H
