//
// Created by Tomas on 15. 11. 2015.
//

#include "../common.h"

namespace Lua {

    ValueObject::ValueObject() {}
    ValueObject::ValueObject(bool b) {
        type = LUA_TBOOLEAN;
        value.b = b;
    };
    ValueObject::ValueObject(double d) {
        type = LUA_TNUMFLT;
        value.d = d;
    }
    ValueObject::ValueObject(int i) {
        type = LUA_TNUMINT;
        value.i = i;
    }
    ValueObject::ValueObject(long long i) {
        type = LUA_TNUMINT;
        value.i = i;
    }
    ValueObject::ValueObject(int t, void* ptr) {
        type = t;
        value.p = ptr;
    }

    ValueObject::ValueObject(const ValueObject& other) {
        type = other.type;

        switch (type) {
            case LUA_TNIL: break;
            case LUA_TBOOLEAN: value.b = other.value.b; break;
            case LUA_TNUMFLT:  value.d = other.value.d; break;
            case LUA_TNUMINT:  value.i = other.value.i; break;
            case LUA_TTABLE:
            case LUA_TSHRSTR:
            case LUA_TLNGSTR:
            case LUA_TCLOSURE: // copy by reference
            case LUA_TNATIVE:
                value.p = other.value.p;
                break;
            default:
                // TODO error
                break;
        }
    }

    ValueObject::~ValueObject() { // TODO dynamic objects are deallocated by GC
    }

    void ValueObject::print() const {
        switch(type) {
            case LUA_TNIL:
                printf("nil\n");
                break;
            case LUA_TBOOLEAN:
                printf("%s\n", (value.b ? "true" : "false"));
                break;
            case LUA_TNUMFLT:
                printf("%lf\n", value.d);
                break;
            case LUA_TNUMINT:
                printf("%lli\n", value.i);
                break;
            case LUA_TSHRSTR:
            case LUA_TLNGSTR:
                ((String*)(value.p))->print();
                break;
            case LUA_TTABLE:
                ((Table*)(value.p))->print();
                break;
            case LUA_TCLOSURE: // TODO
                //((Closure*)(value.p))->print();
                break;
            case LUA_TNATIVE:
                ((Native*)(value.p))->print();
                break;
            default:
                printf("???\n");
                break;
        }
    }


    const char* ValueObject::toString() const {
        char * ptr = new char[50]; // TODO magic constant
        switch(type) {
            case LUA_TNIL: return "nil";
            case LUA_TBOOLEAN: return value.b ? "true" : "false";
            case LUA_TNUMFLT:
                sprintf(ptr, "%f", value.d);
                return ptr;
            case LUA_TNUMINT:
                sprintf(ptr, "%lli", value.i);
                return ptr;
            case LUA_TSHRSTR:
            case LUA_TLNGSTR:
                return ((String*)(value.p))->toString();
            case LUA_TTABLE:
                return "<Table>";
            default:
                assert(false);
                break;
        }
    }



    // TODO fix comparison based on reference, right now strict comparison
    bool ValueObject::operator==(const ValueObject& other) const {

        if (type != other.type) {return false;}

        switch(type) {
            case LUA_TBOOLEAN:
                return value.b == other.value.b;
            case LUA_TNUMFLT:
                return value.d == other.value.d; // TODO epsilon comparison
            case LUA_TNUMINT:
                return value.i == other.value.i;
            case LUA_TSHRSTR:
            case LUA_TLNGSTR:
                return *((String*)value.p) == *((String*)other.value.p);
            case LUA_TTABLE:
            case LUA_TCLOSURE:
            case LUA_TNATIVE:
                return (this == &other);
            default:
                return false;
        }
    }
}
