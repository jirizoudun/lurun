#include "../common.h"

namespace Lua {

    void Native::print() {
        printf("Native\n");
    }

    NativeType Native::getType() {
        return type;
    }


    void Native::call(ValueObject **stack, int npar, int nres) {
        switch (this->type) {
            //TODO: Fix npar, nres = -1 cases.
            case LUA_NAT_PRINT:
                if (npar != 1 || nres != -1) { assert(false); }
                printf("%s\n", stack[1]->toString());
                break;

            case LUA_NAT_ASSERT:
                if (npar != 1 && nres != 0) { assert(false); }
                if (stack[1]->type != LUA_TBOOLEAN) { assert(false); }
                assert(stack[1]->value.b);
                break;

            case LUA_NAT_TOSTRING:
                if (npar != 1) { assert(false); }
                if (nres >= 0) {
                    char *str = to_s(stack[1]);
                    for (int i = 0; i <= nres; i++) {
                        ValueObject *vo = new ValueObject(LUA_TSTRING, new StringObject(str));
                        stack[i] = vo;
                    }
                    delete str;
                }
                break;

            case LUA_NAT_TONUMBER:
                if (npar != 1) { assert(false); }
                if (nres >= 0) {
                    ValueObject vo;
                    if (stack[1]->type == LUA_TSTRING) {
                        char *valueEnd;
                        const char *valueStr = ((StringObject *) stack[1]->value.p)->toString();
                        double value;
                        value = strtod(valueStr, &valueEnd);

                        if (valueStr != valueEnd) {
                            if (fabs(floor(value) - value) < 0.0001) {
                                vo = ValueObject((long long int) value);
                            }
                            else {
                                vo = ValueObject(value);
                            }
                        }
                        else {
                            vo = ValueObject(LUA_TNIL, NULL);
                        }
                    }
                    else if (stack[1]->type == LUA_TNUMFLT || stack[1]->type == LUA_TNUMINT) {
                        vo = *stack[1];
                    }
                    else {
                        vo = ValueObject(LUA_TNIL, NULL);
                    }


                    for (int i = 0; i <= nres; i++) {
                        ValueObject *v = new ValueObject(vo);
                        stack[i] = v;
                    }
                }
                break;

            case LUA_NAT_RAWGET:
                if (npar != 2) { assert(false); }
                if (nres >= 0) {
                    if (stack[1]->type != LUA_TTABLE) { assert(false); }
                    Table *t = ((Table *) stack[1]->value.p);
                    ValueObject vo = t->get(*stack[2]);

                    for(int i = 0; i <= nres; i++) {
                        stack[i] = new ValueObject(vo);
                    }
                }
                break;

            case LUA_NAT_RAWSET:
                if (npar != 3) { assert(false); }
                if (stack[1]->type != LUA_TTABLE) { assert(false); }

                Table *t = ((Table *) stack[1]->value.p);
                t->set(*stack[2], *stack[3]);
                break;
        }
    }

    char *Native::to_s(const ValueObject *vo) {
        char *result;
        switch (vo->type) {
            case LUA_TNIL:
                sprintf(result, "%s", "nil");
                break;

            case LUA_TSTRING:
                result = (char *) ((StringObject *) vo->value.p)->toString();
                break;

            case LUA_TBOOLEAN:
                result = (char *) (vo->value.b ? "true" : "false");
                break;

            case LUA_TNUMINT:
                sprintf(result, "%lld", vo->value.i);
                break;

            case LUA_TNUMFLT:
                sprintf(result, "%f", vo->value.d);
                break;

            default:
                sprintf(result, "%s", "");
                //result = ""; //TODO: Functions, tables, etc
                break;
        }

        char *dyn_result = new char[strlen(result) + 1];
        memcpy(dyn_result, result, strlen(result) * sizeof(char));
        dyn_result[strlen(result)] = 0;

        return dyn_result;
    }
}
