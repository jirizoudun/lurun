//
// Created by Tomas on 15. 11. 2015.
//

#include "../common.h"

namespace Lua {

    Function::Function(String* func_name, int line_defined_first, int line_defined_last, byte num_params,
                       byte is_vararg, byte max_stack_size,
                       InstructionList* code, Container<ValueObject>* constants):
            func_name(func_name),
            line_defined_first(line_defined_first),
            line_defined_last(line_defined_last),
            num_params(num_params),
            is_vararg(is_vararg),
            max_stack_size(max_stack_size),
            code(code),
            constants(constants)
    {}

    Function::~Function() {
        delete func_name;
        delete code;
        delete constants;
    }

    void Function::print() {
        printf("--- Function --------\n");
        printf("Name: "); func_name->print();
        printf("Lines: %i..%i\n", line_defined_first, line_defined_last);
        printf("Params: %i, vararg: %i, max stack: %i\n", num_params, is_vararg, max_stack_size);
        code->print();
        printf("Constants:\n");
        constants->print();
    }
}