#include "../common.h"

namespace VM {

    using namespace Lua;

    VM::VM() {
        stackTop = 0;
        topCallFrame = NULL;
        openUpvals = NULL;
        lastUpval = NULL;

        memset(stack, 0, VM_STACK_SIZE);
    };

    /**
     * Create environment table and other structures needed for VM to run
     */
    void VM::init(Function *initialChunk) {
        printf("\nINIT\n\n");

        // create environment
        Table* _ENV = new Table();
        initEnviroment(_ENV);

        // create upvalue from environment
        stack[stackTop++] = new ValueObject(LUA_TTABLE, (void *)_ENV);
        openUpvals = lastUpval = new UpvalueRef(stack[stackTop-1]); // new upvalue from _ENV table

        // create call frame
        Closure * baseClosure = new Closure(initialChunk);
        baseClosure->upvalues.push_back(lastUpval);

        topCallFrame = new CallFrame(NULL, baseClosure, 1+initialChunk->getMaxStackSize(), 1); // TODO: 1+2 base + max stack
        stackTop += initialChunk->getMaxStackSize();
    }

    void VM::run() {
        topCallFrame->closure->upvalues[0]->getValue()->print();
        execute(topCallFrame);
    }

    void VM::execute(CallFrame * ci) {
        Function* proto = ci->closure->proto;
        InstructionList* code = proto->getCode();

        int base = ci->base;

        for(int ip=0; ip < code->getCount(); ip++) {
            Instruction* inst = code->getInstruction(ip);
            int* args = inst->getArgs();

            int RA = args[0];
            int RB = args[1];
            int RC = args[2];

            //inst->print(); // ; debug

            switch(inst->getOpCode()) {
                case OP_CLOSURE: {
                    Function *p = proto->protos->at(RB);
                    Closure *newClosure = new Closure(p);
                    for(int i = 0; i < p->upvaluesdescs->count; i++)
                    {
                        /* upvalue refers to local variable? */
                        if(p->upvaluesdescs->get(i).instack)
                        {

                        }
                        /* get upvalue from enclosing function */
                        else
                        {

                        }
                    }

//                    newClosure->upvalues.push_back();
                    break;
                }
                case OP_GETTABUP: {
                    ValueObject C;

                    if (RC == ISK(RC)) {
                        C = (*(proto->constants))[INDEXK(RC)];
                        printf("\n\n\n\n");
                        //C.print();
                    } else {
                        // TODO
                        //C = stack[base + RC];
                    };
                    Table* t = (Table*)(ci->closure->upvalues[RB]->getValue()->value.p);
                    stack[base + RA] = new ValueObject(t->get(C));
                    break;
                }
                case OP_SETTABUP:

                    break;
                case OP_GETUPVAL:
                    stack[base + RA] = ci->closure->upvalues[RB]->getValue();
                    break;
                case OP_LOADK:
                    stack[base + RA] = new ValueObject(proto->constants->get(RB)); // TODO refactor the shit out of this fugliness
                    break;
                case OP_LOADBOOL:
                    stack[base + RA] = new ValueObject((bool)RB);

                    if(RC != 0) { ip++; } //TODO: Check this
                    break;
                case OP_CALL: { // R(A), ... ,R(A+C-2) := R(A)(R(A+1), ... ,R(A+B-1))
                    RA = base + RA;
                    RB = base + RB;
                    RC = base + RC;

                    int nres = RC - 2;
                    int npar = RB - 2;

                    // If npar or nres == -1 than theres indeterminate number of parameters or results
                    // example: print(tostring(false))

                    if (stack[RA]->type == LUA_TNATIVE) {
                        ((Native *) (stack[RA]->value.p))->call(stack + RA, npar, nres);
                    } else {
                        // TODO Lua function
                    }
                    break;
                }
                case OP_RETURN:
                    // TODO
                    break;
                default:
                    printf("unknown inst\n");
                    break;
            }

            //printStack(); // ; debug
        }

    }

    void VM::initEnviroment(Table *env) {
        env->set("_G", ValueObject(LUA_TTABLE, (void *) env));
        env->set("print", ValueObject(LUA_TNATIVE, (void *)(new Native(LUA_NAT_PRINT))));
        env->set("assert", ValueObject(LUA_TNATIVE, (void *)(new Native(LUA_NAT_ASSERT))));
        env->set("tonumber", ValueObject(LUA_TNATIVE, (void *)(new Native(LUA_NAT_TONUMBER))));
        env->set("tostring", ValueObject(LUA_TNATIVE, (void *)(new Native(LUA_NAT_TOSTRING))));
        env->set("rawget", ValueObject(LUA_TNATIVE, (void *)(new Native(LUA_NAT_RAWGET))));
        env->set("rawset", ValueObject(LUA_TNATIVE, (void *)(new Native(LUA_NAT_RAWSET))));

        //TODO other native methods

        printf("_ENV:\n");
        env->print();
    }

    void VM::printStack() const {
        printf("\n---- STACK ----------------\n");
        for (int i=0;i<stackTop;i++) {
            printf("%i | ", i);
            if (stack[i] == NULL) {
                printf("NULL\n");
            } else {
                stack[i]->print();
            }
        }
        printf("----/STACK ----------------\n\n");
    }
}
