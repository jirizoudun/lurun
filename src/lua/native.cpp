
#include "../common.h"

namespace Lua {

    void Native::print() {
        printf("Native\n");
    }

    void Native::call(ValueObject** stack, int npar, int nres) {
        switch(this->type) {

            case LUA_NAT_PRINT:
                if (npar != 1 && nres != 0) { assert(false); }
                printf("%s\n", stack[1]->toString());
                break;

            case LUA_NAT_ASSERT:
                if (npar != 1 && nres != 0) { assert(false); }
                if (stack[1]->type != LUA_TBOOLEAN) { assert(false); }
                assert(stack[1]->value.b);
                break;

            case LUA_NAT_TOSTRING:
                if (npar != 1 && nres != 0) { assert(false); }

                break;
        }
    }

    const char * Native::to_s(const ValueObject* vo)
    {
        switch (vo->type)
        {
            case LUA_TBOOLEAN:
                return vo->value.b ? "true" : "false";

            case LUA_TNUMINT:

                break;
        }
    }
}
