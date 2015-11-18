//
// Created by Tomas on 15. 11. 2015.
//

#ifndef LURUN_STRING_H
#define LURUN_STRING_H

namespace Lua {

    class String {

        long long length;
        char * str;
        size_t hash;

        void compute_hash();
    public:
        String(long long length, char* str);
        String(const String& other);
        ~String();

        void print() const;

        size_t getHash()   const;

        bool operator==(const String& other) const;
    };
}

#endif //LURUN_STRING_H
