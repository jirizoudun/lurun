//
// Created by Jiří Zoudun on 22/11/15.
//

#include <stdio.h>
#include "native.h"

namespace Lua {
    void Native::printToConsole(const char *text) {
        if (text) {
            printf("%s", text);
        }
    }

    void Native::print() {
        printf("Native\n");
    }
}
