//
// Created by Tomas on 15. 11. 2015.
//

#ifndef LURUN_FUNCTION_H
#define LURUN_FUNCTION_H

namespace Lua {

    class Function {

        String* func_name;
        int line_defined_first;
        int line_defined_last;

        byte num_params;
        byte is_vararg;
        byte max_stack_size;

        InstructionList* code;
        Container<ValueObject>* constants;

        // upvalues
        // protos

    public:
        Function(String* func_name, int line_defined_first, int line_defined_last, byte num_params, byte is_vararg,
                 byte max_stack_size, InstructionList *code, Container<ValueObject>* constants);
        ~Function();

        void print();

    };

}

#endif //LURUN_FUNCTION_H
