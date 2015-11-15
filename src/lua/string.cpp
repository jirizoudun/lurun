//
// Created by Tomas on 15. 11. 2015.
//

#include "../common.h"

namespace Lua {

    String::String(size_t length, char *str):
            length(length),
            str(str)
    {}

    String::~String() {
        delete [] str;
    }

    void String::print() {
        printf("%s\n", str);
    }
}
