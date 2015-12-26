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
#if DEBUG
        printf("\nINIT\n\n");
#endif

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

#if DEBUG
            printf("###################################\n");
            inst->print(); // ; debug
            printf("###################################\n");
#endif

            switch(inst->getOpCode()) {
                case OP_CLOSURE: {
                    Function * p = proto->protos->at(RB);
                    Closure * newClosure = new Closure(p);

                    for(int i = 0; i < p->upvaluesdescs->count; i++) {
                        int instack = p->upvaluesdescs->get(i).instack;
                        int idx = p->upvaluesdescs->get(i).idx;

                        // upvalue refers to local variable?
                        if (instack) {
                            // TODO wtf to do here?
                        // get upvalue from enclosing function
                        } else {
                            newClosure->upvalues.push_back(ci->closure->upvalues[idx]);
                        }
                    }

                    (stack+base)[RA] = new ValueObject(LUA_TCLOSURE, (void*)newClosure);
                    break;
                }
                case OP_GETTABUP: {
                    ValueObject C = getVO(stack + base, proto, RC);
                    Table* t = (Table*)(ci->closure->upvalues[RB]->getValue()->value.p);

                    stack[base + RA] = new ValueObject(t->get(C));
                    break;
                }
                case OP_SETTABUP: { // UpValue[A][RK(B)] := RK(C)
                    ValueObject B = getVO(stack + base, proto, RB);
                    ValueObject C = getVO(stack + base, proto, RC);

                    ((Table *)(ci->closure->upvalues[RA]->getValue()->value.p))->set(B, C);
                    break;
                }
                case OP_LOADK:
                    stack[base + RA] = new ValueObject(proto->constants->get(RB)); // TODO refactor the shit out of this fugliness
                    break;
                case OP_LOADBOOL:
                    stack[base + RA] = new ValueObject((bool)RB);

                    if(RC != 0) { ip++; } //TODO: Check this
                    break;
                case OP_CALL: { // R(A), ... ,R(A+C-2) := R(A)(R(A+1), ... ,R(A+B-1))
                    /* TODO
                        if (B == 0) then B = top.
                        If (C == 0), then 'top' is set to last_result+1
                        so next open instruction (OP_CALL, OP_RETURN, OP_SETLIST) may use 'top'.
                     */

                    if (RB == 0) {
                        assert(false); // TODO
                    }
                    if (RC == 0) {
                        assert(false); // TODO
                    }

                    RA = base + RA;

                    int nres = RC - 2;
                    int npar = RB - 1;

                    if (stack[RA]->type == LUA_TNATIVE) {
                        ((Native *) (stack[RA]->value.p))->call(stack + RA, npar, nres);
                    } else {
                        Closure*  nc = ((Closure*)(stack[RA]->value.p));
                        Function* np = nc->proto;

                        topCallFrame = new CallFrame(ci, nc, ci->top + np->getMaxStackSize(), ci->top, npar, nres);
                        stackTop += np->getMaxStackSize();
                        execute(topCallFrame);

                        // return from call frame
                        delete topCallFrame;
                        topCallFrame = ci;
                    }
                    break;
                }
                case OP_RETURN: /* return R(A), ... ,R(A+B-2) */
                    /* TODO
                     * In OP_RETURN, if (B == 0) then return up to 'top'.
                     */

                    if (RB == 0) {
                        assert(false);
                    }

                    // TODO return

                    return;
                case OP_MOVE: // RA = RB
                    stack[base + RA] = stack[base + RB];
                    break;
                case OP_CONCAT: { // R(A) := R(B).. ... ..R(C)
                    const char *B = stack[base + RB]->toString();
                    const char *C = stack[base + RC]->toString();

                    size_t B_len = strlen(B);
                    size_t C_len = strlen(C);

                    char *concat = new char[B_len + C_len];
                    memcpy(concat, B, B_len * sizeof(char));
                    memcpy(concat + B_len, C, C_len * sizeof(char));

                    String* str = new String(B_len + C_len, concat);
                    stack[base + RA] = new ValueObject(LUA_TSHRSTR, str); // TODO LUA_T_LNGSTR
                    break;
                }
                default:
                    printf("unknown inst\n");
                    break;
            }

#if DEBUG
            printStack(ci); // ; debug
#endif
        }

    }

    ValueObject VM::getVO(ValueObject** stack, Function * proto, int R) const {
        if (ISK(R)) {
            return (*(proto->constants))[INDEXK(R)];
        } else {
            return *(stack[R]);
        }
    }

    void VM::initEnviroment(Table *env) {
        env->set("_G", ValueObject(LUA_TTABLE, (void *) env));
        env->set("print", ValueObject(LUA_TNATIVE, (void *)(new Native(LUA_NAT_PRINT))));
        env->set("assert", ValueObject(LUA_TNATIVE, (void *)(new Native(LUA_NAT_ASSERT))));

        //TODO other native methods

#if DEBUG
        printf("_ENV:\n");
        env->print();
#endif
    }

    void VM::printStack(CallFrame * ci) const {
        printf("\n---- STACK ----------------\n");
        for (int i=0;i<stackTop;i++) {
            printf("%i | ", i - ci->base);
            if (stack[i] == NULL) {
                printf("NULL\n");
            } else {
                stack[i]->print();
            }
        }
        printf("----/STACK ----------------\n\n");
    }
}
