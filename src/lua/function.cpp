//
// Created by Tomas on 15. 11. 2015.
//

#include "../common.h"

namespace Lua {

    Function::Function(String* func_name, int line_defined_first, int line_defined_last, byte num_params,
                       byte is_vararg, byte max_stack_size,
                       InstructionList* code,
                       Container<ValueObject>* constants, Container<Upvalue>* upvalues, std::vector<Function*>* protos):
            func_name(func_name),
            line_defined_first(line_defined_first),
            line_defined_last(line_defined_last),
            num_params(num_params),
            is_vararg(is_vararg),
            max_stack_size(max_stack_size),
            code(code),
            constants(constants),
            upvalues(upvalues),
            protos(protos)
    {}

    Function::~Function() {
        delete func_name;
        delete code;
        delete constants;
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
        upvalues->print();
        printf("Protos:\n", protos->size());
        for (int i=0; i<protos->size(); i++) {
            (*protos)[i]->print();
        }
    }
}