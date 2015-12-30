
#include "../common.h"

namespace Lua {

    InstructionList::InstructionList(int count, Instruction** list):
            count(count),
            list(list)
    {}

    InstructionList::~InstructionList() {
        for (int i=0; i<count; i++) {
            delete list[i];
        }
        delete [] list;
    }

    void InstructionList::print() {
        printf("Instructions:\n");
        for(int i=0; i<count; i++) {
            printf("\t");
            list[i]->print();
        }
    }

    int InstructionList::getCount() const {
        return count;
    }
    Instruction* InstructionList::getInstruction(const int index) const {
        return list[index];
    }
}