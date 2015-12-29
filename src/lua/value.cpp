
#include "../common.h"

namespace Lua {

    ValueObject::ValueObject() {
        type = LUA_TNIL;
    }
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
            case LUA_TSTRING:
            case LUA_TCLOSURE: // copy by reference
            case LUA_TNATIVE:
                value.p = other.value.p;
                break;
            default:
                printf("Can't copy ValueObject [%i]\n", type);
                assert(false);
                break;
        }
    }

    ValueObject& ValueObject::operator=(const ValueObject& other) {
        type = other.type;
        value = other.value;
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
            case LUA_TSTRING:
                ((StringObject *)(value.p))->print();
                break;
            case LUA_TTABLE:
                ((Table*)(value.p))->print();
                break;
            case LUA_TCLOSURE:
                printf("[%i] Closure\n", value.p);
                break;
            case LUA_TNATIVE:
                ((Native*)(value.p))->print();
                break;
            default:
                break;
        }
    }


    const string ValueObject::toString() const {

        switch(type) {
            case LUA_TNIL:     return string("nil");
            case LUA_TBOOLEAN: return string(value.b ? "true" : "false");
            case LUA_TSTRING:  return ((StringObject *)(value.p))->getString();
        }

        char * ptr = new char[50]; // TODO magic constant
        switch(type) {
            case LUA_TNUMFLT:  sprintf(ptr, "%g", value.d); break;
            case LUA_TNUMINT:  sprintf(ptr, "%lli", value.i); break;
            case LUA_TTABLE:   sprintf(ptr, "Table [%d]", ((Native*)(value.p))->getType());   break;
            case LUA_TNATIVE:  sprintf(ptr, "Native [%d]", ((Native*)(value.p))->getType());  break;
            case LUA_TCLOSURE: sprintf(ptr, "Closure [%d]", ((Native*)(value.p))->getType()); break;
            default:
                printf("Can't convert to string  type %i\n", type);
                assert(false);
                break;
        }

        string res = string(ptr);
        delete [] ptr;
        return res;
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
            case LUA_TSTRING:
                return ((StringObject *)value.p)->getHash() == ((StringObject *)other.value.p)->getHash();
            case LUA_TTABLE:
            case LUA_TCLOSURE:
            case LUA_TNATIVE:
                return (value.p == other.value.p);
                // return (this == &other);
            default:
                return false;
        }
    }
}
