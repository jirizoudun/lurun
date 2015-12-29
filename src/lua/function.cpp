//
// Created by Tomas on 15. 11. 2015.
//

#include "../common.h"

namespace Lua {

    Function::Function(StringObject * func_name, int line_defined_first, int line_defined_last, byte num_params,
                       byte is_vararg, byte max_stack_size,
                       InstructionList* code,
                       Container<ValueObject>* constants, Container<UpvalueDesc>* upvalues, std::vector<Function*>* protos):
            func_name(func_name),
            line_defined_first(line_defined_first),
            line_defined_last(line_defined_last),
            num_params(num_params),
            is_vararg(is_vararg),
            max_stack_size(max_stack_size),
            code(code),
            constants(constants),
            upvaluesdescs(upvalues),
            protos(protos)
    {}

    Function::~Function() {
        delete func_name;
        delete code;
        delete constants;
        delete upvaluesdescs;

        for (std::vector<Function*>::iterator it = protos->begin(); it != protos->end(); ++it) {
            delete *it;
        }
        delete protos;
    }

    void Function::print() {
        printf("--- Function --------\n");
        if (func_name != NULL) {
            printf("Name: "); func_name->print();
        }
        printf("Lines: %i..%i\n", line_defined_first, line_defined_last);
        printf("Params: %i, vararg: %i, max stack: %i\n", num_params, is_vararg, max_stack_size);
        code->print();
        printf("Constants:\n");
        constants->print();
        printf("Upvalues:\n");
        upvaluesdescs->print();
        printf("Protos: %lu\n", protos->size());
        for (int i=0; i<protos->size(); i++) {
            (*protos)[i]->print();
        }
    }

    InstructionList* Function::getCode() const {
        return code;
    }
    byte Function::getMaxStackSize() const {
        return max_stack_size;
    }
}