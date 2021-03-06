//
// Created by Jiří Zoudun on 27/12/15.
//

#ifndef LURUN_FILE_H
#define LURUN_FILE_H

#include <String>
#include "../common.h"

namespace Lua {
    class File: public GCObject {
        bool opened;
        char path[MAX_FILENAME_LENGTH];
        FILE *f;

        Table* getFileMetatable();

    public:
        Table* metatable = NULL;

        File();
        ~File();

        bool isOpened();
        bool open(const char* path, const char *mode);
        bool close();

        bool write(std::string str);
        bool readLine(std::string &res);

        std::string getPathString();

        void print();

        void gc() const;
    };
}

#endif //LURUN_FILE_H
