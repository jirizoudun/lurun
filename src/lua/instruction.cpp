//
// Created by Tomas on 15. 11. 2015.
//

#include "../common.h"

namespace Lua {

    struct OpCodeInfo {
        OpCodeInfo(const char *name, Lua::InstructionType type) : name(name), type(type) { };

        const char *name;
        Lua::InstructionType type;
    };

/// StringObject to suppress C++11 warnings.
/// Converting ENUM OpCode to string.
    static OpCodeInfo OpCodeInfoList[OPCODES_COUNT] = {
            OpCodeInfo("OP_MOVE", Lua::ABC),
            OpCodeInfo("OP_LOADK", Lua::ABX),
            OpCodeInfo("OP_LOADKX", Lua::ABC),
            OpCodeInfo("OP_LOADBOOL", Lua::ABC),
            OpCodeInfo("OP_LOADNIL", Lua::ABC),
            OpCodeInfo("OP_GETUPVAL", Lua::ABC),
            OpCodeInfo("OP_GETTABUP", Lua::ABC),
            OpCodeInfo("OP_GETTABLE", Lua::ABC),
            OpCodeInfo("OP_SETTABUP", Lua::ABC),
            OpCodeInfo("OP_SETUPVAL", Lua::ABC),
            OpCodeInfo("OP_SETTABLE", Lua::ABC),
            OpCodeInfo("OP_NEWTABLE", Lua::ABC),
            OpCodeInfo("OP_SELF", Lua::ABC),
            OpCodeInfo("OP_ADD", Lua::ABC),
            OpCodeInfo("OP_SUB", Lua::ABC),
            OpCodeInfo("OP_MUL", Lua::ABC),
            OpCodeInfo("OP_MOD", Lua::ABC),
            OpCodeInfo("OP_POW", Lua::ABC),
            OpCodeInfo("OP_DIV", Lua::ABC),
            OpCodeInfo("OP_IDIV", Lua::ABC),
            OpCodeInfo("OP_BAND", Lua::ABC),
            OpCodeInfo("OP_BOR", Lua::ABC),
            OpCodeInfo("OP_BXOR", Lua::ABC),
            OpCodeInfo("OP_SHL", Lua::ABC),
            OpCodeInfo("OP_SHR", Lua::ABC),
            OpCodeInfo("OP_UNM", Lua::ABC),
            OpCodeInfo("OP_BNOT", Lua::ABC),
            OpCodeInfo("OP_NOT", Lua::ABC),
            OpCodeInfo("OP_LEN", Lua::ABC),
            OpCodeInfo("OP_CONCAT", Lua::ABC),
            OpCodeInfo("OP_JMP", Lua::ASBX),
            OpCodeInfo("OP_EQ", Lua::ABC),
            OpCodeInfo("OP_LT", Lua::ABC),
            OpCodeInfo("OP_LE", Lua::ABC),
            OpCodeInfo("OP_TEST", Lua::ABC),
            OpCodeInfo("OP_TESTSET", Lua::ABC),
            OpCodeInfo("OP_CALL", Lua::ABC),
            OpCodeInfo("OP_TAILCALL", Lua::ABC),
            OpCodeInfo("OP_RETURN", Lua::ABC),
            OpCodeInfo("OP_FORLOOP", Lua::ASBX),
            OpCodeInfo("OP_FORPREP", Lua::ASBX),
            OpCodeInfo("OP_TFORCALL", Lua::ABC),
            OpCodeInfo("OP_TFORLOOP", Lua::ASBX),
            OpCodeInfo("OP_SETLIST", Lua::ABC),
            OpCodeInfo("OP_CLOSURE", Lua::ABX),
            OpCodeInfo("OP_VARARG", Lua::ABC),
            OpCodeInfo("OP_EXTRAARG", Lua::AX)};

    Instruction::Instruction(int inst) {
        this->decode(inst);
    }

    void Instruction::print() {
        printf("(%d) %s ", this->instruction, OpCodeInfoList[this->instruction].name);

        switch (this->type)
        {
            case ABC:
                printf("%d %d %d", this->args[0], this->args[1], this->args[2]);
                break;

            case ABX:
                printf("%d %d", this->args[0], this->args[1]);
                break;

            case ASBX:
                printf("%d %d", this->args[0], this->args[1]);
                break;

            case AX:
                printf("%d", this->args[0]);
                break;

            default:
                printf("Unrecognized type!");
                break;
        }

        printf ("\n");
    }

    void Instruction::decode(int inst) {
        this->instruction = GET_OPCODE(inst);
        this->type = OpCodeInfoList[this->instruction].type;

        //Args
        switch (OpCodeInfoList[this->instruction].type) {
            case ABC:
                this->args[0] = GETARG_A(inst);
                this->args[1] = GETARG_B(inst);
                this->args[2] = GETARG_C(inst);
                break;

            case ABX:
                this->args[0] = GETARG_A(inst);
                this->args[1] = GETARG_Bx(inst);
                break;

            case ASBX:
                this->args[0] = GETARG_A(inst);
                this->args[1] = GETARG_sBx(inst);
                break;

            case AX:
                this->args[0] = GETARG_Ax(inst);
                break;

            default: //FIXME
                break;
        }
    }

    OpCode Instruction::getOpCode() {
        return instruction;
    }
    int* Instruction::getArgs() {
        return args;
    }
}