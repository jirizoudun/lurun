#include "../common.h"

namespace VM {

    using namespace Lua;

    VM::VM() {
        stackTop = 0;
        topCallFrame = NULL;
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
        lastUpval = new UpvalueRef(stack + stackTop - 1, NULL); // new upvalue from _ENV table

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

                        // upvalue refers to local variable of enclosing function
                        if (instack) {

                            ValueObject ** ptr = stack + base + idx;

                            // find upval
                            UpvalueRef * upval = lastUpval;
                            while (upval != NULL) {
                                if (upval->voPointer == ptr) {
                                    newClosure->upvalues.push_back(upval);
                                    break;
                                } else {
                                    upval = upval->next;
                                }
                            }

                            // upval wasn't found, create new one
                            if (upval == NULL) {
                                lastUpval = new UpvalueRef(ptr, lastUpval);
                                newClosure->upvalues.push_back(lastUpval);
                            }
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

                    ((Table *) (ci->closure->upvalues[RA]->getValue()->value.p))->set(B, C);
                    break;
                }
                case OP_GETUPVAL:
                    stack[base + RA] = ci->closure->upvalues[RB]->getValue();
                    break;

                case OP_SETUPVAL: { // UpValue[B] := R(A)
                    UpvalueRef *upv = ci->closure->upvalues[RB];
                    if (upv->voPointer == NULL) {
                        upv->value = *stack[base + RA];
                    } else {
                        upv->voPointer = stack + base + RA;
                    }
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
                        RB = ci->top + 1; // TODO not sure if correct but seems to work
                    }

                    RA = base + RA;

                    int last_result = RC - 2;
                    int last_param  = RB - 1;

                    // If npar or nres == -1 then there's indeterminate number of parameters or results
                    // example: print(tostring(false))

                    if (stack[RA]->type == LUA_TNATIVE) {
                        ((Native *) (stack[RA]->value.p))->call(stack + RA, last_param, last_result);
                    } else {
                        Closure*  nc = ((Closure*)(stack[RA]->value.p));
                        Function* np = nc->proto;

                        topCallFrame = new CallFrame(ci, nc, RA + np->getMaxStackSize() + 1, RA+1, last_param, last_result);
                        execute(topCallFrame);

                        // return from call frame
                        delete topCallFrame;
                        topCallFrame = ci;
                    }

                    if (RC == 0) {
                        ci->top = RA + last_result + 1;
                    }
                    break;
                }
                case OP_RETURN: { /* return R(A), ... ,R(A+B-2) */
                    /* TODO
                     * In OP_RETURN, if (B == 0) then return up to 'top'.
                     */

                    if (RB == 0) {
                        assert(false);
                    }

                    // close upvalues
                    for(int R = base; R < base+ci->top; R++) {
                        if (stack[R] == NULL || stack[R]->type != LUA_TCLOSURE) {continue;}

                        Closure * clClosure = (Closure*)stack[R]->value.p;
                        Function* clProto = clClosure->proto;
                        for(int i = 0; i < clProto->upvaluesdescs->count; i++) {

                            if (clProto->upvaluesdescs->get(i).instack) {
                                UpvalueRef * toClose = clClosure->upvalues[i];
                                if (toClose == lastUpval) {
                                    lastUpval = toClose->next;
                                }
                                toClose->close();
                            }
                        }
                    }

                    // move from my registers to parent registers
                    int R = base - 1;
                    for (int i = RA; i <= RA + RB - 2; i++) {
                        stack[R++] = stack[base + i];
                    }
                    return;
                }
                case OP_MOVE: // RA = RB
                    stack[base + RA] = stack[base + RB];
                    break;
                case OP_CONCAT: { // R(A) := R(B).. ... ..R(C)
                    const char *B = stack[base + RB]->toString();
                    const char *C = stack[base + RC]->toString();

                    string* concat = new string(B);
                    concat->append(C);

                    stack[base + RA] = new ValueObject(LUA_TSTRING, new StringObject(concat));
                    break;
                }

                case OP_ADD: {
                    ValueObject B = getVO(stack + base, proto, RB);
                    ValueObject C = getVO(stack + base, proto, RC);
                    ValueObject * res;

                    assert(B.type == LUA_TNUMINT || B.type == LUA_TNUMFLT
                        || C.type == LUA_TNUMINT || C.type == LUA_TNUMFLT);

                    if (B.type == LUA_TNUMFLT && C.type == LUA_TNUMFLT) {

                        res = new ValueObject(B.value.d + C.value.d);

                    } else if (B.type == LUA_TNUMFLT || C.type == LUA_TNUMFLT) {
                        if (B.type == LUA_TNUMINT) {
                            res = new ValueObject(B.value.i + C.value.d);
                        } else {
                            res = new ValueObject(B.value.d + C.value.i);
                        }
                    } else {
                        res = new ValueObject(B.value.i + C.value.i);
                    }

                    stack[base + RA] = res;
                    break;
                }
                default:
                    printf("unknown inst\n");
                    assert(false);
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
        env->set("tonumber", ValueObject(LUA_TNATIVE, (void *)(new Native(LUA_NAT_TONUMBER))));
        env->set("tostring", ValueObject(LUA_TNATIVE, (void *)(new Native(LUA_NAT_TOSTRING))));
        env->set("rawget", ValueObject(LUA_TNATIVE, (void *)(new Native(LUA_NAT_RAWGET))));
        env->set("rawset", ValueObject(LUA_TNATIVE, (void *)(new Native(LUA_NAT_RAWSET))));

        //TODO other native methods

#if DEBUG
        printf("_ENV:\n");
        env->print();
#endif
    }

    void VM::printStack(CallFrame * ci) const {
        printf("\n---- STACK ----------------\n");
        for (int i=0;i<ci->top;i++) {
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
