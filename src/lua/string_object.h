
#ifndef LURUN_STRING_H
#define LURUN_STRING_H

namespace Lua {

    using std::string;

    class StringObject {

        string* str;
        size_t hash;

        void compute_hash();
    public:
        StringObject(string str);
        StringObject(string* str);
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
