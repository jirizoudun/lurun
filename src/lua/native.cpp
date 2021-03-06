#include <iostream>
#include "../common.h"

namespace Lua {

    void Native::print() {
        printf("Native\n");
    }

    NativeType Native::getType() {
        return type;
    }

    // base_res needed for TFORCALL
    int Native::call(ValueObject *stack, int npar, ValueObject *base_res, int nres) {
        switch(this->type) {

            case LUA_NAT_PRINT:
                assert(nres == 0 && npar >= 1);
                for(int i=1; i<=npar; i++) {
                    printf("%s", stack[i].toString().c_str());
                    printf(i+1 <= npar ? "\t" : "\n");
                }
                return 0;

            case LUA_NAT_TYPE: {
                assert(npar == 1);

                string type;
                switch(stack[1].type) {
                    case LUA_TNIL:     type = "nil";     break;
                    case LUA_TNUMFLT:
                    case LUA_TNUMINT:  type = "number";  break;
                    case LUA_TSTRING:  type = "string";  break;
                    case LUA_TBOOLEAN: type = "boolean"; break;
                    case LUA_TTABLE:   type = "table";   break;
                    case LUA_TCLOSURE: type = "closure"; break;
                    default:
                        type = "unknown";
                }

                base_res[0] = ValueObject(LUA_TSTRING, ALLOC_STRING(type));

                return 1;
            }

            case LUA_NAT_ASSERT:
                assert(npar == 1 && nres == 0);
                assert(!IS_NIL(stack[1]));
                assert(!IS_BOOL(stack[1]) || VO_B(stack[1]));
                return 0;

            case LUA_NAT_TOSTRING:
                assert(npar == 1);
                assert(nres == -1 || nres == 1);
                base_res[0] = ValueObject(LUA_TSTRING, ALLOC_STRING(stack[1].toString()));
                return 1;

            case LUA_NAT_TONUMBER:
//                assert(false); // TODO check
                if (npar != 1) { assert(false); }
                if (nres >= 0) {
                    ValueObject vo;
                    if (IS_STRING(stack[1])) {
                        char *valueEnd;
                        const char *valueStr = ((StringObject*)VO_P(stack[1]))->toString();
                        double value;
                        value = strtod(valueStr, &valueEnd);

                        if (valueStr != valueEnd) {
                            if (fabs(floor(value) - value) < 0.0001) {
                                vo = ValueObject((long long int) value);
                            } else {
                                vo = ValueObject(value);
                            }
                        }
                        else {
                            vo = ValueObject();
                        }
                    } else if (stack[1].type == LUA_TNUMFLT || stack[1].type == LUA_TNUMINT) {
                        vo = stack[1];
                    } else {
                        vo = ValueObject();
                    }

                    for (int i = 0; i <= nres; i++) {
                        base_res[0] = vo;
                    }
                }
                break;

            /**
             * gets table and key
             * returns next key and value
             */
            case LUA_NAT_NEXT: {
                assert(npar == 2);
                std::pair<ValueObject,ValueObject> res = ((Table*)VO_P(stack[1]))->next(stack[2]);

                base_res[0] = res.first;
                base_res[1] = res.second;
                return 2;
            }

            /*
             * gets table
             * returns iterator function (next), table and stopping condition (nil)
             */
            case LUA_NAT_PAIRS:
                assert(npar == 1);
                base_res[0] = ValueObject(LUA_TNATIVE, ALLOC_NATIVE(LUA_NAT_NEXT));
                base_res[2] = ValueObject();
                return 3;

            case LUA_NAT_GETMETATABLE: {
                assert(npar == 1);
                Table* metatable = ((Table*)VO_P(stack[1]))->metatable;
                if (metatable != NULL) {
                    base_res[0] = ValueObject(LUA_TTABLE, metatable);
                }
                return 1;
            }
            case LUA_NAT_SETMETATABLE: {
                assert(npar == 2);
                // TODO If the original metatable has a "__metatable" field, raise an error.

                Table* t = (Table*)VO_P(stack[1]);
                if (IS_NIL(stack[2])) {
                    t->metatable = NULL;
                } else {
                    t->metatable = (Table*)VO_P(stack[2]);
                }
                return 0;
            }

            case LUA_NAT_IO_WRITE:
                assert(nres == 0 && npar >= 1);
                for(int i=1; i<=npar; i++) {
                    printf("%s", stack[i].toString().c_str());
                    printf(i+1 <= npar ? "\t" : "");
                }
                return 0;

            case LUA_NAT_IO_READ: {
                assert(nres >= 1 && npar == 0);
                string line;
                getline(std::cin, line);

                for (int i = 0; i < nres; i++) {
                    base_res[i] = ValueObject(LUA_TSTRING, ALLOC_STRING(line));
                }
                return nres;
            }

            case LUA_NAT_IO_OPEN: {
                assert(npar == 2);
                assert(IS_STRING(stack[1]) && IS_STRING(stack[2]));
                File *f = (File*)ALLOC_FILE();
                f->open(((StringObject *) VO_P(stack[1]))->getString().c_str(),
                        ((StringObject *) VO_P(stack[2]))->getString().c_str());

                for(int i = 0; i < nres; i++) {
                    base_res[i] = ValueObject(LUA_TFILE, f);
                }

                return nres;
            }

            case LUA_NAT_IO_CLOSE: {
                assert(npar == 1);
                assert(IS_FILE(stack[1]));

                File *f = (File *) VO_P(stack[1]);
                if (f->isOpened()) {
                    f->close();
                }
                break;
            }

            case LUA_NAT_FILE_WRITE: {
                assert(npar == 2);
                assert(IS_FILE(stack[1]));

                File *f = ((File*)VO_P(stack[1]));
                if(f->isOpened()) {
                    if(IS_STRING(stack[2]) || IS_NUMERIC(stack[2])) {
                        f->write(stack[2].toString());
                    } else {
                        // TODO: Error
                        assert(false);
                    }
                }
                else{
                    // TODO Write some notice that file is closed.
                    assert(false);
                }

                break;
            }

            case LUA_NAT_FILE_READ: {
                assert(npar == 1 && IS_FILE(stack[1]));

                File *f = (File*)VO_P(stack[1]);
                if(f->isOpened()) {
                    string line;
                    if(f->readLine(line)) {
                        base_res[0] = ValueObject(LUA_TSTRING, ALLOC_STRING(line));
                    }
                    else
                    {
                        base_res[0] = ValueObject(); // LUA_TNIL
                    }
                }
                else {
                    printf("File isn't open!\n");
                    base_res[0] = ValueObject();
                }
                return 1;
            }

            case LUA_NAT_MATH_CEIL: {
                assert(npar == 1);
                base_res[0] = ValueObject((int)ceil(VO_D(stack[1])));
                return 1;
            }

            case LUA_NAT_STRING_SUB: {
                assert(npar == 3 && IS_STRING(stack[1]));
                string str = ((StringObject*)VO_P(stack[1]))->toString();

                long long pos = VO_I(stack[2]) - 1; // Lua indexing from 1
                long long len = VO_I(stack[3]) - 1 - pos + 1;

                string sub = str.substr(pos, len);
                base_res[0] = ValueObject(LUA_TSTRING, ALLOC_STRING(sub));
                return 1;
                break;
            }

            default: {
                assert(false);
                return 0; // make compiler happy
            }
        }
        return 0;
    }
}
