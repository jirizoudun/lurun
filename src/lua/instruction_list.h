
#ifndef LURUN_INSTRUCTION_LIST_H
#define LURUN_INSTRUCTION_LIST_H

namespace Lua {

    class InstructionList {

        int count;
        Instruction** list;

    public:
        InstructionList(int count, Instruction** list);
        ~InstructionList();

        void print();

        int getCount() const;
        Instruction* getInstruction(const int index) const;
    };

}


#endif //LURUN_INSTRUCTION_LIST_H
