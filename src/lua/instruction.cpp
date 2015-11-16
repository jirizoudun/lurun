//
// Created by Tomas on 15. 11. 2015.
//

#include "../common.h"

/*
** size and position of opcode arguments.
*/
#define SIZE_C		9
#define SIZE_B		9
#define SIZE_Bx		(SIZE_C + SIZE_B)
#define SIZE_A		8
#define SIZE_Ax		(SIZE_C + SIZE_B + SIZE_A)

#define SIZE_OP		6

#define POS_OP		0
#define POS_A		(POS_OP + SIZE_OP)
#define POS_C		(POS_A + SIZE_A)
#define POS_B		(POS_C + SIZE_C)
#define POS_Bx		POS_C
#define POS_Ax		POS_A

#define MAXARG_Bx        ((1<<SIZE_Bx)-1)
#define MAXARG_sBx        (MAXARG_Bx>>1)         /* 'sBx' is signed */

/* creates a mask with 'n' 1 bits at position 'p' */
#define MASK1(n,p)	((~((~(unsigned int)0)<<(n)))<<(p))

#define getarg(i,pos,size)	(((int)((i)>>pos)) & MASK1(size,0))

#define GET_OPCODE(i)	((OpCode)((i>>POS_OP) & MASK1(SIZE_OP,0)))

#define GETARG_A(i)	getarg(i, POS_A, SIZE_A)
#define GETARG_B(i)	getarg(i, POS_B, SIZE_B)
#define GETARG_C(i)	getarg(i, POS_C, SIZE_C)
#define GETARG_Bx(i)	getarg(i, POS_Bx, SIZE_Bx)
#define GETARG_Ax(i)	getarg(i, POS_Ax, SIZE_Ax)
#define GETARG_sBx(i)	(GETARG_Bx(i)-MAXARG_sBx)



namespace Lua {

    struct OpCodeInfo {
        OpCodeInfo(const char *name, Lua::InstructionType type) : name(name), type(type) { };

        const char *name;
        Lua::InstructionType type;
    };

/// String to suppress C++11 warnings.
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
}