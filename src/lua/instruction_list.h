//
// Created by Tomas on 15. 11. 2015.
//

#ifndef LURUN_INSTRUCTION_LIST_H
#define LURUN_INSTRUCTION_LIST_H

namespace Lua {

    class InstructionList {

        int count;
        Instruction** list;

    public:

        InstructionList(int count, Instruction** list);
        ~InstructionList();
    };

}


#endif //LURUN_INSTRUCTION_LIST_H
