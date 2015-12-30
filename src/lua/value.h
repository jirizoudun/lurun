
#ifndef LURUN_VALUE_H
#define LURUN_VALUE_H

#define LUA_TNIL        0
#define LUA_TBOOLEAN    1
#define LUA_TNUMFLT     3
#define LUA_TNUMINT     19
#define LUA_TSHRSTR     4   // WARNING: used only for reading, never used in VM, use LUA_TSTRING instead
#define LUA_TLNGSTR     20  // WARNING: used only for reading, never used in VM, use LUA_TSTRING instead
#define LUA_TSTRING     100
#define LUA_TTABLE		5
#define LUA_TCLOSURE    6
#define LUA_TNATIVE     99

#define IS_NUMERIC(O) (IS_INT(O) || IS_FLT(O))
#define IS_INT(O)     ((O).type == LUA_TNUMINT)
#define IS_FLT(O)     ((O).type == LUA_TNUMFLT)
#define IS_NIL(O)     ((O).type == LUA_TNIL)
#define IS_BOOL(O)    ((O).type == LUA_TBOOLEAN)
#define IS_TABLE(O)   ((O).type == LUA_TTABLE)
#define IS_NATIVE(O)  ((O).type == LUA_TNATIVE)
#define IS_STRING(O)  ((O).type == LUA_TSTRING)
#define IS_CLOSURE(O) ((O).type == LUA_TCLOSURE)

#define VO_P(VO) ((VO).value.p)
#define VO_I(VO) ((VO).value.i)
#define VO_D(VO) ((VO).value.d)
#define VO_B(VO) ((VO).value.b)

namespace Lua {

    using namespace std;

    // inspired by http://www.lua.org/doc/jucs05.pdf, page 5
    union Value {
    public:
        void*       p;
        long long   i;
        double      d;
        bool        b;
    };

    class ValueObject {
    public:
        int type;
        Value value;

        ValueObject();
        ValueObject(bool b);
        ValueObject(double d);
        ValueObject(int i);
        ValueObject(long long i);
        ValueObject(int type, void* ptr);

        ValueObject(const ValueObject&);
        ValueObject& operator= (const ValueObject& other);

        ~ValueObject();
        void print() const;
        const string toString() const;

        bool operator==(const ValueObject& other) const;
    };

    struct ValueObjectHasher {
        size_t operator()(const ValueObject& o) const {
            switch (o.type) {
                case LUA_TNIL:
                    return std::hash<void*>()(NULL);
                case LUA_TBOOLEAN:
                    return std::hash<bool>()(VO_B(o));
                case LUA_TNUMFLT:
                    return std::hash<double>()(VO_D(o));
                case LUA_TNUMINT:
                    return std::hash<long long>()(VO_I(o));
                case LUA_TSTRING:
                    return ((StringObject*)VO_P(o))->getHash();
                case LUA_TTABLE:
                    return std::hash<long long>()((long long)VO_P(o));
                case LUA_TCLOSURE:
                case LUA_TNATIVE:
                default:
                    printf("Can't set as table key");
                    assert(false);
            }
        }
    };
}

#endif //LURUN_VALUE_H
