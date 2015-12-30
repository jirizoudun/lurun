
#ifndef LURUN_STRING_H
#define LURUN_STRING_H

namespace Lua {

    using std::string;

    class StringObject {

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
    };
}

#endif //LURUN_STRING_H
