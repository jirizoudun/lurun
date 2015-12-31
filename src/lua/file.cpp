//
// Created by Jiří Zoudun on 27/12/15.
//

#include <string.h>
#include "file.h"

using namespace std;

namespace Lua {
    File::File() {
        f = NULL;
        opened = false;
        path[0] = 0;

        metatable = getFileMetatable();
    }

    File::~File() {

    }

    bool File::open(const char *path, const char *mode) {
        if(this->opened)
        {
            return false;
        }

        if(strlen(path) >= MAX_FILENAME_LENGTH)
        {
            return false;
        }

        f = fopen(path, mode);

        if(f != NULL)
        {
            this->opened = true;
            strncpy(this->path, path, MAX_FILENAME_LENGTH - 1);
            this->path[MAX_FILENAME_LENGTH - 1] = 0;
        }

        return this->opened;
    }

    bool File::close() {
        if(!this->opened)
        {
            return false;
        }

        this->opened = false;

        if(f == NULL)
        {
            return false;
        }

        fclose(f);
        return true;
    }

    bool File::isOpened() {
        return this->opened;
    }

    bool File::write(std::string str) {
        if(!this->opened)
        {
            return false;
        }

        fprintf(f, str.c_str());
        return true;
    }

    bool File::readLine(string &res) {
        if(!this->opened)
        {
            return false;
        }

        char line[MAX_FILE_LINE_LENGTH];

        if (fgets(line, MAX_FILE_LINE_LENGTH, f) != NULL) {
            res.clear();
            res.append(line);
            res.replace(res.end()-1, res.end(), ""); // Remove new line at the end.
            return true;
        }
        else {
            return false;
        }
    }

    void File::print()
    {
        printf("File (%s)\n", opened ? path : "closed");
    }

    std::string File::getPathString()
    {
        return string(path);
    }

    Table* File::getFileMetatable()
    {
        Table *t = (Table*)ALLOC_TABLE();
        t->set("write", ValueObject(LUA_TNATIVE, ALLOC_NATIVE(LUA_NAT_FILE_WRITE)));
        t->set("read", ValueObject(LUA_TNATIVE, ALLOC_NATIVE(LUA_NAT_FILE_READ)));
        return t;
    }

    void File::gc() const {
        if (metatable != NULL) {
            VM::HeapManager::markGray((char*)metatable);
        }
    }
}
