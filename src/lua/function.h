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

    public:

        Container<ValueObject>* constants;
        Container<UpvalueDesc>* upvaluesdescs;
        std::vector<Function*>* protos;

        Function(String* func_name, int line_defined_first, int line_defined_last, byte num_params, byte is_vararg,
                 byte max_stack_size, InstructionList *code,
                 Container<ValueObject>* constants, Container<UpvalueDesc>* upvalues, std::vector<Function*>* protos);
        ~Function();

        InstructionList* getCode() const;
        byte getMaxStackSize() const;

        void print();

    };

}

#endif //LURUN_FUNCTION_H
