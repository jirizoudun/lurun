//
// Created by Tomas on 15. 11. 2015.
//

#ifndef LURUN_STRING_H
#define LURUN_STRING_H

namespace Lua {

    class String {

        long long length;
        char * str;

    public:
        String(long long length, char* str);
        ~String();
        void print();
    };
}

#endif //LURUN_STRING_H
