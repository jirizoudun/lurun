
#include "../common.h"

namespace VM {

    using namespace Lua;

    VM::VM() {
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
        Table* _ENV = (Table*)ALLOC_TABLE();
        initEnviroment(_ENV);

        // create upvalue from environment
        stack[0] = ValueObject(LUA_TTABLE, (void *)_ENV);
        lastUpval = (UpvalueRef*)ALLOC_UPVAL(stack, NULL); // new upvalue from _ENV table

        // create call frame
        Closure * baseClosure = (Closure*)ALLOC_CLOSURE(initialChunk);
        baseClosure->upvalues->push_back(lastUpval);

        topCallFrame = new CallFrame(NULL, baseClosure, 1, 2);
    }

    void VM::run() {
        execute(topCallFrame);

        /*
        GC::root(stack, topCallFrame->top, topCallFrame);
        GC::mark();
        GC::sweep();
         */

        //GC::root(stack, topCallFrame->top, NULL);
        //GC::mark();
        GC::sweep();

        //HeapManager::print();

        // delete initial frame
        delete topCallFrame;
    }

    void VM::execute(CallFrame * ci) {
        Function* proto = ci->closure->proto;
        InstructionList* code = proto->getCode();

        int base = ci->base;
        int params = ci->npar;

        // clear stack
        for (int R=base+params; R < ci->base + ci->size; R++) {
            stack[R].type = LUA_TNIL;
        }
        //

        for(int ip=0; ip < code->getCount(); ip++) {
            Instruction* inst = code->getInstruction(ip);
            int* args = inst->getArgs();

            int RA = args[0];
            int RB = args[1];
            int RC = args[2];

#if DEBUG
            printf("### ");
            printf("%i: ", ip);
            inst->print(); // ; debug
#endif

            OpCode op = inst->getOpCode();
            switch(op) {
                case OP_CLOSURE: {
                    Function * p = proto->protos->at(RB);
                    Closure * newClosure = (Closure*)ALLOC_CLOSURE(Closure(p));

                    for(int i = 0; i < p->upvaluesdescs->count; i++) {
                        int instack = p->upvaluesdescs->get(i).instack;
                        int idx = p->upvaluesdescs->get(i).idx;

                        // upvalue refers to local variable of enclosing function
                        if (instack) {

                            ValueObject * ptr = stack + base + idx;

                            // find upval
                            UpvalueRef * upval = lastUpval;
                            while (upval != NULL) {
                                if (upval->voPointer == ptr) {
                                    newClosure->upvalues->push_back(upval);
                                    break;
                                } else {
                                    upval = upval->next;
                                }
                            }

                            // upval wasn't found, create new one
                            if (upval == NULL) {
                                lastUpval = (UpvalueRef*)ALLOC_UPVAL(ptr, lastUpval);
                                newClosure->upvalues->push_back(lastUpval);
                            }
                        // get upvalue from enclosing function
                        } else {
                            newClosure->upvalues->push_back(ci->closure->upvalues->at(idx));
                        }
                    }

                    (stack+base)[RA] = ValueObject(LUA_TCLOSURE, (void*)newClosure);
                    break;
                }
                case OP_GETTABUP: { // R(A) := UpValue[B][RK(C)]
                    ValueObject C = getVO(stack + base, proto, RC);
                    Table* t = (Table*)VO_P(ci->closure->upvalues->at(RB)->getValue());

                    stack[base + RA] = ValueObject(t->get(C));
                    break;
                }
                case OP_SETTABUP: { // UpValue[A][RK(B)] := RK(C)
                    ValueObject B = getVO(stack + base, proto, RB);
                    ValueObject C = getVO(stack + base, proto, RC);
                    ((Table *)VO_P(ci->closure->upvalues->at(RA)->getValue()))->set(B, C);
                    break;
                }
                case OP_GETUPVAL:
                    stack[base + RA] = ci->closure->upvalues->at(RB)->getValue();
                    break;

                case OP_LOADNIL: // R(A), R(A+1), ..., R(A+B) := nil
                    for (int R=base + RA; R<=base + RA + RB; R++) {
                        stack[R].type = LUA_TNIL;
                    }
                    break;

                case OP_SETUPVAL: { // UpValue[B] := R(A)
                    UpvalueRef *upv = ci->closure->upvalues->at(RB);
                    if (upv->voPointer == NULL) {
                        upv->value = stack[base + RA];
                    } else {
                        upv->voPointer = stack + base + RA;
                    }
                    break;
                }
                case OP_LOADK:
                    stack[base + RA] = ValueObject(proto->constants->get(RB)); // TODO refactor the shit out of this fugliness
                    break;
                case OP_LOADBOOL:
                    stack[base + RA] = ValueObject((bool)RB);
                    if(RC != 0) { ip++; } //TODO: Check this
                    break;


                case OP_JMP: { // pc+=sBx; if (A) close all upvalues >= R(A - 1)

                    if (RA != 0) {
                        assert(false); // TODO
                    }

                    ip += RB;
                    break;
                }

                case OP_EQ: case OP_LT: case OP_LE: {
                    ValueObject B = getVO(stack + base, proto, RB);
                    ValueObject C = getVO(stack + base, proto, RC);

                    bool res;

                    if (B.type == LUA_TSTRING && C.type == LUA_TSTRING) {
                        res = cmp<>(((StringObject*)B.value.p)->getString(), ((StringObject*)C.value.p)->getString(), op);
                    } else if (IS_NUMERIC(B) && IS_NUMERIC(C)) {
                        res = cmp<>((IS_INT(B) ? B.value.i : B.value.d),
                                    (IS_INT(C) ? C.value.i : C.value.d),
                                    op);
                    } else if (B.type == LUA_TBOOLEAN || C.type == LUA_TBOOLEAN) {
                        res = cmp<>(B.value.b, C.value.b, op);
                    } else if (B.type == LUA_TNIL || C.type == LUA_TNIL) {
                        res = cmp<>(B.type, C.type, op);
                    } else {
                        res = cmp<>(B.value.p, C.value.p, op);
                    }

                    if (res != (bool)RA) {++ip;}
                    break;
                }

                case OP_TEST: { // if not (R(A) <=> C) then pc++
                    ValueObject A = stack[base + RA];
                    bool isFalse = (IS_NIL(A) || (IS_BOOL(A) && !A.value.b));
                    if (RC == isFalse) {++ip;}
                    break;
                }

                case OP_CALL: { // R(A), ... ,R(A+C-2) := R(A)(R(A+1), ... ,R(A+B-1))
                    /* If (B == 0) then B = top.
                     * If (C == 0), then 'top' is set to last_result+1
                     * so next open instruction (OP_CALL, OP_RETURN, OP_SETLIST) may use 'top'.
                     */

                    int nres = RC - 1;
                    int npar = RB - 1;

                    if (RB == 0) {
                        npar = ci->top - base - RA - 1; // npar or RB?
                    }

                    RA = base + RA;

                    int top;
                    if (IS_NATIVE(stack[RA])) {
                        top = RA - 1 + ((Native *) (stack[RA].value.p))->call(stack + RA, npar, stack + RA, nres);
                    } else {
                        Closure*  nc = ((Closure*)sgetptr(RA));
                        topCallFrame = new CallFrame(ci, nc, RA + 1, RA + npar + 1, npar, nres);
                        execute(topCallFrame);
                        top = topCallFrame->top;

                        // return from call frame
                        delete topCallFrame;
                        topCallFrame = ci;
                    }

                    if (RC == 0) {
                        ci->top = top + 1;
                    }
                    break;
                }

                case OP_TFORCALL: { // R(A+3), ... ,R(A+2+C) := R(A)(R(A+1), R(A+2));
                    int nres = RA + 2 + RC;
                    RA = base + RA;
                    if (IS_NATIVE(stack[RA])) {
                        ((Native *) (stack[RA].value.p))->call(stack + RA, 2, stack + RA + 3, nres);
                    } else {
                        printf("Can't TFORCALL Lua function");
                        assert(false);
                    }
                    break;
                }

                case OP_RETURN: { /* return R(A), ... ,R(A+B-2) */
                    if (RB == 0) {
                        // TODO if (B == 0) then return up to 'top'.
                        assert(false);
                    }

                    // close upvalues
                    for(int R = base; R < base+ci->size; R++) { // TODO ci->size or ci->top?
                        if (!IS_CLOSURE(stack[R])) {continue;} // stack[R] == NULL ||

                        Closure * clClosure = (Closure*)sgetptr(R);
                        Function* clProto = clClosure->proto;
                        for(int i = 0; i < clProto->upvaluesdescs->count; i++) {

                            if (clProto->upvaluesdescs->get(i).instack) {
                                UpvalueRef * toClose = clClosure->upvalues->at(i);
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

                    ci->top = R - 1;
                    return;
                }
                case OP_MOVE: // RA = RB
                    stack[base + RA] = stack[base + RB];
                    break;
                case OP_CONCAT: { // R(A) := R(B).. ... ..R(C)
                    string B = stack[base + RB].toString();
                    string C = stack[base + RC].toString();
                    stack[base + RA] = ValueObject(LUA_TSTRING, ALLOC_STRING(string(B).append(C)));
                    break;
                }

                case OP_SETTABLE: { // R(A)[RK(B)] := RK(C)
                    ValueObject B = getVO(stack+base, proto, RB);
                    ValueObject C = getVO(stack+base, proto, RC);

                    ((Table*)sgetptr(base+RA))->set(B, C);
                    break;
                }
                case OP_NEWTABLE: {
                    // TODO sizes from RB, RC to optimize memory and speed
                    stack[base + RA] = ValueObject(LUA_TTABLE, ALLOC_TABLE());
                    break;
                }
                case OP_GETTABLE: { // R(A) := R(B)[RK(C)]
                    ValueObject C = getVO(stack+base, proto, RC);
                    ValueObject B = getVO(stack+base, proto, RB);
                    switch(B.type)
                    {
                        case LUA_TFILE:
                            stack[base + RA] = ValueObject(((File*)sgetptr(base + RB))->metatable->get(C));
                            break;

                        default:
                            stack[base + RA] = ValueObject(((Table*)sgetptr(base + RB))->get(C));
                            break;
                    }
                    break;
                }

                case OP_ADD: case OP_SUB:
                case OP_MUL: case OP_DIV: {
                    ValueObject B = getVO(stack + base, proto, RB);
                    ValueObject C = getVO(stack + base, proto, RC);

                    ValueObject res;
                    if ((IS_NUMERIC(B) && IS_NUMERIC(C))) {
                        if (B.type == LUA_TNUMFLT && C.type == LUA_TNUMFLT) {
                            res = arithmetic(B.value.d, C.value.d, op);
                        } else if (B.type == LUA_TNUMFLT || C.type == LUA_TNUMFLT) {
                            if (B.type == LUA_TNUMINT) {
                                res = arithmetic((double)B.value.i, C.value.d, op);
                            } else {
                                res = arithmetic(B.value.d, (double)C.value.i, op);
                            }
                        } else {
                            res = arithmetic(B.value.i, C.value.i, op);
                        }
                        stack[base + RA] = res;
                    } else if (B.type == LUA_TTABLE || C.type == LUA_TTABLE) {
                        // TODO other operations
                        if (!callM(ci, B, C, RA, "__add")
                         && !callM(ci, C, B, RA, "__add")) {
                            printf("Can't invoke operation\n");
                            assert(false);
                        }
                    } else {
                        printf("Can't invoke arithmetic operation\n");
                        assert(false);
                    }
                    break;
                }

                case OP_FORPREP: { // R(A)-=R(A+2); pc+=sBx

                    ValueObject start = stack[base + RA];
                    ValueObject limit = stack[base + RA + 1];
                    ValueObject step  = stack[base + RA + 2];

                    // TODO should check limits here

                    if (!IS_NUMERIC(start) || !IS_NUMERIC(limit) || !IS_NUMERIC(step)) {
                        printf("Loop control must have numeric attributes\n");
                        assert(false);
                    }

                    ip += RB;
                    break;
                }

                case OP_FORLOOP: { // R(A) += R(A+2); if R(A) <?= R(A+1) then { pc+=sBx; R(A+3)=R(A) }

                    ValueObject start = stack[base + RA];
                    ValueObject limit = stack[base + RA + 1];
                    ValueObject step  = stack[base + RA + 2];

                    if (IS_INT(start) && IS_INT(limit) && IS_INT(step)) {
                        long long istart = VO_I(start);
                        long long ilimit = VO_I(limit);

                        bool cnd = VO_I(step) > 0 ? istart <= ilimit : ilimit <= istart;
                        if (cnd) {
                            ip += RB;
                            stack[base + RA + 3] = ValueObject(start);
                        }
                        VO_I(stack[base + RA]) += VO_I(step);

                    // float
                    } else {
                        printf("Can't do for with floats");
                        assert(false);
                    }
                    break;
                }

                case OP_TFORLOOP: { // if R(A+1) ~= nil then { R(A)=R(A+1); pc += sBx }
                    RA = base + RA;
                    if (!IS_NIL(stack[RA + 1])) {
                        stack[RA] = stack[RA + 1];
                        ip += RB;
                    }
                    break;
                }

                case OP_SETLIST: { // R(A)[(C-1)*FPF+i] := R(A+i), 1 <= i <= B
                    // TODO will probably break for more elements, use array instead of hash?

                    int start = (RC-1) * LFIELDS_PER_FLUSH;
                    Table * t = ((Table*)sgetptr(base + RA));

                    for (int i=1; i<=RB; i++) {
                        t->set(ValueObject(start+i), stack[base + RA + i]);
                    }
                    t->setLen(RB);
                    break;
                }
                case OP_LEN: { // R(A) := length of R(B)
                    assert(IS_TABLE(stack[base + RB]));
                    stack[base + RA] = ValueObject(((Table*)sgetptr(base + RB))->getLen());
                    break;
                }

                case OP_SELF: { // R(A+1) := R(B); R(A) := R(B)[RK(C)]

                    ValueObject C = getVO(stack + base, proto, RC);

                    stack[base + RA + 1] = ValueObject(stack[base + RB]);
                    stack[base + RA] = ValueObject(((Table*)sgetptr(base + RB))->get(C));
                    break;
                }

                default:
                    printf("unknown inst\n");
                    inst->print();
                    assert(false);
                    break;
            }


            /*
            if (inst->getOpCode() == OP_JMP) {
                HeapManager::print();
            }*/

            GC::root(stack, ci);
            GC::mark();

            //HeapManager::printStatus();

            if (HeapManager::gray.empty()) {
                GC::sweep();
            }
            //GC::sweep();

#if DEBUG_STACK
            printStack(ci); // ; debug
#endif
        }

    }

    ValueObject VM::getVO(ValueObject* stack, Function * proto, int R) const {
        if (ISK(R)) {
            return (*(proto->constants))[INDEXK(R)];
        } else {
            return stack[R];
        }
    }

    bool VM::callM(CallFrame* ci, ValueObject B, ValueObject C, int RA, const char* m) {

        Table* meta = ((Table*)B.value.p)->metatable;
        if (meta == NULL) { meta = ((Table*)C.value.p)->metatable; }
        if (meta == NULL) { return false; }

        ValueObject func = meta->get(m);
        if (IS_NIL(func)) {
            return false;
        } else {
            Closure*  nc = ((Closure*)(func.value.p));

            int newbase = ci->base + ci->size;
            stack[newbase]     = B;
            stack[newbase + 1] = C;

            topCallFrame = new CallFrame(ci, nc, newbase, newbase+2, 2, 1);
            execute(topCallFrame);

            stack[ci->base + RA] = stack[topCallFrame->top];

            // return from call frame
            delete topCallFrame;
            topCallFrame = ci;
        }
        return true;
    }

    ValueObject VM::arithmetic(long long a, long long b, OpCode op) {
        switch(op) {
            case OP_ADD: return ValueObject(a + b);
            case OP_SUB: return ValueObject(a - b);
            case OP_MUL: return ValueObject(a * b);
            case OP_DIV: return ValueObject((double)a / (double)b);
            default:
                printf("unknown arithmetic op");
                assert(false);
        }
    }
    ValueObject VM::arithmetic(double a, double b, OpCode op) {
        double res;
        switch(op) {
            case OP_ADD: res = a + b; break;
            case OP_SUB: res = a - b; break;
            case OP_MUL: res = a * b; break;
            case OP_DIV: res = a / b; break;
            default:
                printf("unknown arithmetic op");
                assert(false);
        }
        return ValueObject(res);
    }

    template <class T, class U>
    bool VM::cmp(T a, U b, OpCode op) {
        switch(op) {
            case OP_EQ: return a == b;
            case OP_LT: return a < b;
            case OP_LE: return a <= b;
            default:
                printf("unknown cmp op");
                assert(false);
        }
    }

    void VM::initEnviroment(Table *env) {
       BaseEnv::initEnviroment(env);

        //TODO other native methods

#if DEBUG
        printf("_ENV:\n");
        env->print();
#endif
    }

    void VM::printStack(CallFrame * ci) const {
        printf("\n---- STACK -----------------\n");
        for (int i=0;i < ci->stack_max;i++) {
            if (i == ci->base + ci->size) {
                printf("+\n");
            }
            printf("%3i ", i - ci->base);
            printf((i == ci->top ? "> " : "| "));
            /*if (stack[i] == NULL) {
                printf("NULL\n");
            } else {*/
                stack[i].print();
            //}
        }
        printf("----/STACK ----------------\n\n");

    }
}
