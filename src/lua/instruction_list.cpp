//
// Created by Tomas on 15. 11. 2015.
//

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
}