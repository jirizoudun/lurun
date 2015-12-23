//
// Created by Tomas on 15. 11. 2015.
//

#include "../common.h"

namespace Lua {

    String::String(long long length, char *str):
            length(length),
            str(str)
    {
        compute_hash();
    }
    String::String(const String& other) {
        length = other.length;
        str = new char[length];
        strncpy(str, other.str, length); // TODO watch out! length!
        compute_hash();
    }

    String::~String() {
        delete [] str;
    }

    void String::compute_hash() {
        using std::string;
        hash = std::hash<string>()(string(str));
    }

    void String::print() const {
        printf("<%zu> %s\n", hash, str);
    }
    const char* String::toString() const {
        return str;
    }
    size_t String::getHash() const {
        return hash;
    }

    bool String::operator==(const String& other) const {
        return (strcmp(str, other.str) == 0);
    }
}
