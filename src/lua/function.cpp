//
// Created by Tomas on 15. 11. 2015.
//

#include "../common.h"

namespace Lua {

    Function::Function(String* func_name, int line_defined_first, int line_defined_last, byte num_params,
                       byte is_vararg, byte max_stack_size,
                       InstructionList* code):
            func_name(func_name),
            line_defined_first(line_defined_first),
            line_defined_last(line_defined_last),
            num_params(num_params),
            is_vararg(is_vararg),
            max_stack_size(max_stack_size),
            code(code)
    {}

    Function::~Function() {
        delete func_name;
        delete code;
    }
}