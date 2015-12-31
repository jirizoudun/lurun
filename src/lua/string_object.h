
#ifndef LURUN_STRING_H
#define LURUN_STRING_H

namespace Lua {

    using std::string;

    class StringObject: public GCObject {

        /*
         * Each StringObject creates it's own copy of string, for simplicity
         * Reference is destroyed when StringObject is destroyed,
         * so we need to keep track only about StringObject
         * String are immutable */
        string* str;

        size_t hash;

        void compute_hash();
    public:
        StringObject(string str);
        StringObject(const char* str);
        StringObject(const StringObject & other);
        ~StringObject();

        void print() const;
        const char* toString() const;
        string getString() const;

        size_t getHash()   const;

        bool operator==(const StringObject & other) const;

        void gc() const {
            // Do nothing, has no pointers to mark gray
            // str is unique for this Object and does not need to be GC'd
        }
    };
}

#endif //LURUN_STRING_H
