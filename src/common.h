//
// Created by Tomas on 15. 11. 2015.
//

#ifndef LURUN_COMMON_H
#define LURUN_COMMON_H

#define VERBOSE 1

#define MAX_FILENAME_LENGTH 128
#define BUFFER_SIZE 1048576 //MB

#define INSTRUCTION_SIZE 4

/**
 * Compiled lua file info with buffer for file content.
 */
struct LFILE{
    char filename[MAX_FILENAME_LENGTH + 1];
    unsigned char buffer[BUFFER_SIZE];
    unsigned int buffer_size;
};

typedef unsigned char uchar;
typedef unsigned char byte;


#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "lua/lua.h"
#include "header.h"
#include "reader.h"

#endif //LURUN_COMMON_H
