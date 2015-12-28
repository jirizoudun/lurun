#include "../common.h"
#include "base_enviroment.h"

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
        Table* _ENV = new Table();
        initEnviroment(_ENV);

        // create upvalue from environment
        stack[0] = new ValueObject(LUA_TTABLE, (void *)_ENV);
        lastUpval = new UpvalueRef(stack, NULL); // new upvalue from _ENV table

        // create call frame
        Closure * baseClosure = new Closure(initialChunk);
        baseClosure->upvalues.push_back(lastUpval);

        topCallFrame = new CallFrame(NULL, baseClosure, 1, 2);
    }

    void VM::run() {
        execute(topCallFrame);
    }

    void VM::execute(CallFrame * ci) {
        Function* proto = ci->closure->proto;
        InstructionList* code = proto->getCode();

        int base = ci->base;
        int params = ci->npar;

        // clear stack
        ValueObject* nilPtr = new ValueObject(); // TODO no need to create new ptr each time
        for (int R=base+params; R < ci->base + ci->size; R++) {
            stack[R] = nilPtr;
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
                case OP_GETTABUP: { // R(A) := UpValue[B][RK(C)]
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

                case OP_LOADNIL: // R(A), R(A+1), ..., R(A+B) := nil
                    for (int R=base + RA; R<=base + RA + RB; R++) {
                        stack[R] = new ValueObject();
                    }
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
                    } else if (IS_NUMERIC(B.type) && IS_NUMERIC(C.type)) {
                        res = cmp<>((IS_INT(B) ? B.value.i : B.value.d),
                                    (IS_INT(C) ? C.value.i : C.value.d),
                                    op);
                    } else {
                        res = cmp<>(B.value.p, C.value.p, op);
                    }

                    if (res != (bool)RA) {++ip;}
                    break;
                }

                case OP_TEST: { // if not (R(A) <=> C) then pc++
                    ValueObject* A = stack[base + RA];
                    bool isFalse = (A->type == LUA_TNIL || (A->type == LUA_TBOOLEAN && !A->value.b));
                    if ((RC && isFalse) || (!RC && !isFalse)) {++ip;}
                    break;
                }

                case OP_CALL: { // R(A), ... ,R(A+C-2) := R(A)(R(A+1), ... ,R(A+B-1))
                    /* If (B == 0) then B = top.
                     * If (C == 0), then 'top' is set to last_result+1
                     * so next open instruction (OP_CALL, OP_RETURN, OP_SETLIST) may use 'top'.
                     */

                    if (RB == 0) {
                        RB = ci->top;
                    }

                    RA = base + RA;

                    int last_result = RC - 2;
                    int last_param  = RB - 1;

                    // If npar or nres == -1 then there's indeterminate number of parameters or results
                    // example: print(tostring(false))

                    if (stack[RA]->type == LUA_TNATIVE) {
                        ((Native *) (stack[RA]->value.p))->call(stack + RA, last_param, stack + RA, last_result);
                    } else {
                        Closure*  nc = ((Closure*)(stack[RA]->value.p));
                        Function* np = nc->proto;

                        topCallFrame = new CallFrame(ci, nc, RA + 1, RA + last_param + 1, last_param, last_result);
                        execute(topCallFrame);

                        // return from call frame
                        delete topCallFrame;
                        topCallFrame = ci;
                    }

                    if (RC == 0) {
                        ci->top = RA + last_result + 2;
                    }
                    break;
                }

                case OP_TFORCALL: { // R(A+3), ... ,R(A+2+C) := R(A)(R(A+1), R(A+2));
                    int nres = RA + 2 + RC;
                    //R5, .., R9 := R2(R3, R4)

                    RA = base + RA;

                    if (stack[RA]->type == LUA_TNATIVE) {
                        ((Native *) (stack[RA]->value.p))->call(stack + RA, 2, stack + RA + 3, nres);
                    } else {
                        printf("Can't TFORCALL Lua function");
                        assert(false);
                    }
                    break;
                }

                case OP_RETURN: { /* return R(A), ... ,R(A+B-2) */
                    /* TODO
                     * In OP_RETURN, if (B == 0) then return up to 'top'.
                     */

                    if (RB == 0) {assert(false);}

                    // close upvalues
                    for(int R = base; R < base+ci->size; R++) { // TODO ci->size or ci->top?
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

                case OP_SETTABLE: { // R(A)[RK(B)] := RK(C)
                    ValueObject B = getVO(stack+base, proto, RB);
                    ValueObject C = getVO(stack+base, proto, RC);

                    //B.print();
                    //C.print();

                    ((Table*)(stack[base + RA]->value.p))->set(B, C);
                    break;
                }
                case OP_NEWTABLE: {
                    // TODO sizes from RB, RC
                    stack[base + RA] = new ValueObject(LUA_TTABLE, new Table());
                    break;
                }
                case OP_GETTABLE: { // R(A) := R(B)[RK(C)]
                    ValueObject C = getVO(stack+base, proto, RC);
                    stack[base + RA] = new ValueObject(((Table*)(stack[base + RB]->value.p))->get(C));
                    break;
                }

                case OP_ADD: case OP_SUB: {
                    ValueObject B = getVO(stack + base, proto, RB);
                    ValueObject C = getVO(stack + base, proto, RC);

                    assert(IS_NUMERIC(B.type) && IS_NUMERIC(C.type));
                    ValueObject * res;

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
                    break;
                }

                case OP_FORPREP: { // R(A)-=R(A+2); pc+=sBx

                    ValueObject* start = stack[base + RA];
                    ValueObject* limit = stack[base + RA + 1];
                    ValueObject* step  = stack[base + RA + 2];

                    // TODO should check limits here

                    if (!(IS_NUMERIC(start->type) && IS_NUMERIC(limit->type) && IS_NUMERIC(step->type))) {
                        printf("Loop control must have numeric attributes\n");
                        assert(false);
                    }

                    ip += RB;
                    break;
                }

                case OP_FORLOOP: { // R(A) += R(A+2); if R(A) <?= R(A+1) then { pc+=sBx; R(A+3)=R(A) }

                    ValueObject* start = stack[base + RA];
                    ValueObject* limit = stack[base + RA + 1];
                    ValueObject* step  = stack[base + RA + 2];

                    if (start->type == LUA_TNUMINT && limit->type == LUA_TNUMINT && step->type == LUA_TNUMINT) {
                        long long istart = start->value.i + step->value.i;
                        long long ilimit = limit->value.i;
                        start->value.i = istart;

                        if (istart < ilimit) {
                            ip += RB;
                            stack[base + RA + 3] = new ValueObject(*start);
                        }

                    // float
                    } else {
                        printf("Can't do for with floats");
                        assert(false);
                    }
                    break;
                }

                case OP_TFORLOOP: { // if R(A+1) ~= nil then { R(A)=R(A+1); pc += sBx }
                    RA = base + RA;
                    if (stack[RA + 1]->type != LUA_TNIL) {
                        stack[RA] = stack[RA + 1];
                        ip += RB;
                    }
                    break;
                }

                case OP_SETLIST: { // R(A)[(C-1)*FPF+i] := R(A+i), 1 <= i <= B
                    // TODO will probably break for more elements, use array instead of hash?

                    int start = (RC-1) * LFIELDS_PER_FLUSH;
                    Table * t = ((Table*)stack[base + RA]->value.p);

                    for (int i=1; i<=RB; i++) {
                        t->set(ValueObject(start+i), *stack[base + RA + i]);
                    }
                    t->setLen(RB);
                    break;
                }
                case OP_LEN: { // R(A) := length of R(B)
                    assert(stack[base + RB]->type == LUA_TTABLE);
                    stack[base + RA] = new ValueObject(((Table*)(stack[base+RB]->value.p))->getLen());
                    break;
                }

                default:
                    printf("unknown inst\n");
                    inst->print();
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

    ValueObject* VM::arithmetic(long long a, long long b, OpCode op) {
        long long res;
        switch(op) {
            case OP_ADD: res = a + b; break;
            case OP_SUB: res = a - b; break;
            default:
                printf("unknown arithmetic op");
                assert(false);
        }
        return new ValueObject(res);
    }
    ValueObject* VM::arithmetic(double a, double b, OpCode op) {
        double res;
        switch(op) {
            case OP_ADD: res = a + b; break;
            case OP_SUB: res = a - b; break;
            default:
                printf("unknown arithmetic op");
                assert(false);
        }
        return new ValueObject(res);
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
        for (int i=0;i < ci->base + ci->size;i++) {
            printf("%3i ", i - ci->base);
            printf((i == ci->top ? "> " : "| "));
            if (stack[i] == NULL) {
                printf("NULL\n");
            } else {
                stack[i]->print();
            }
        }
        printf("----/STACK ----------------\n\n");

    }
}
