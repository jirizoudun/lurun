#include <stdio.h>
#include <string.h>

#include "common.h"
#include "header.h"
#include "reader.h"

#define TEST_FILE "../../luac.out"

#define ARG_COUNT 2
#define ARG_FILENAME 1

#define ERROR_NO_FILE "No input file given!\nUsage:\nbinaryFile [filename]\n"
#define ERROR_INV_FILE "Invalid input file given!\nPlease, provide valid path to file.\n"
#define ERROR_HEADER_FORMAT "Invalid format of header!\nPlease, provide file eith valid header.\n"
#define ERROR_AT_BYTE "Error at byte [%d]\n"

#include <string>
#include <iostream>

/**
 * Prints binary representation of char.
 */
void printbinchar(unsigned char character)
{
    for (int i = (sizeof(char)*8) - 1; i >= 0; --i)
    {
        putchar( (character & (1 << i)) ? '1' : '0' );
    }
}

/**
 * Loading of file to buffer in @c LFILE.
 */
bool loadfile(LFILE *lfile)
{
    FILE *f = fopen(lfile->filename, "rb"); //read, binary
    unsigned char c = 0; //current char
    int i = 0; //index in file

    lfile->buffer_size = 0;

    if(!f)
    {
        return false;
    }

    while(f && feof(f) == 0 && lfile->buffer_size < BUFFER_SIZE)
    {
        fread(&c, 1, 1, f);
        lfile->buffer[lfile->buffer_size++] = c;
    }

    fclose(f);

    return true;
}

// Returns -1 if everything is ok otherwise int >= 0 indicating position in file where
// problem occurs.
int checkheader(LFILE *lfile)
{
    int i = 0; //current postion in buffer
    bool retval = true;

    //signature
    retval = checkheader_signature(lfile->buffer + i, SIGNATURE_SIZE);
    if(!retval)
    {
        return i;
    }
    i += SIGNATURE_SIZE;

    //version
    retval = checkheader_version(lfile->buffer + i, VERSION_SIZE);
    if(!retval)
    {
        return i;
    }
    i += VERSION_SIZE;

    //format
    retval = checkheader_format(lfile->buffer + i, FORMAT_SIZE);
    if(!retval)
    {
        return i;
    }
    i += FORMAT_SIZE;

    //luacdata
    retval = checkheader_luacdata(lfile->buffer + i, LUACDATA_SIZE);
    if(!retval)
    {
        return i;
    }
    i += LUACDATA_SIZE;

    //intsize
    retval = checkheader_intsize(lfile->buffer + i, INTSIZE_SIZE);
    if(!retval)
    {
        return i;
    }
    i += INTSIZE_SIZE;

    //sizetsize
    retval = checkheader_sizetsize(lfile->buffer + i, SIZETSIZE_SIZE);
    if(!retval)
    {
        return i;
    }
    i += SIZETSIZE_SIZE;

    //instructionsize
    retval = checkheader_instructionsize(lfile->buffer + i, INSTRUCTIONSIZE_SIZE);
    if(!retval)
    {
        return i;
    }
    i += INSTRUCTIONSIZE_SIZE;

    //luaintsize
    retval = checkheader_luaintsize(lfile->buffer + i, LUAINTSIZE_SIZE);
    if(!retval)
    {
        return i;
    }
    i += LUAINTSIZE_SIZE;

    //luanumbersize
    retval = checkheader_luanumbersize(lfile->buffer + i, LUANUMBERSIZE_SIZE);
    if(!retval)
    {
        return i;
    }
    i += LUANUMBERSIZE_SIZE;

    //luacint
    retval = checkheader_luacint(lfile->buffer + i, LUACINT_SIZE);
    if(!retval)
    {
        return i;
    }
    i += LUACINT_SIZE;

    //luacnum
    retval = checkheader_luacnum(lfile->buffer + i, LUACNUM_SIZE);
    if(!retval)
    {
        return i;
    }
    i += LUACNUM_SIZE;

    //sizeupvalue
    retval = checkheader_sizeupvalue(lfile->buffer + i, SIZEUPVALUE_SIZE);
    if(!retval)
    {
        return i;
    }
    i += SIZEUPVALUE_SIZE;


    return -1;
}


int main(int argc, char **argv) {
#if VERBOSE
    printf("Loader started\n");
#endif

    bool retval;

    if(argc < ARG_COUNT)
    {
        fputs(ERROR_NO_FILE, stderr);
        return -1;
    }

    //Copy filename and load file to buffer
    LFILE lfile;
    strncpy(lfile.filename, argv[ARG_FILENAME], MAX_FILENAME_LENGTH);
    lfile.filename[MAX_FILENAME_LENGTH] = 0;
    retval = loadfile(&lfile);
    if(!retval)
    {
        fputs(ERROR_INV_FILE, stderr);
        return -1;
    }

    //Check header
    int error_pos = checkheader(&lfile);
    if(error_pos != -1)
    {
        fputs(ERROR_HEADER_FORMAT, stderr);
        fprintf(stderr, ERROR_AT_BYTE, error_pos);
        return -1;
    }

#if VERBOSE
    printf("OK!\n");
#endif

    // decode file
    Reader* reader = new Reader(lfile);
    Lua::Function* f = reader->readFile();

#if VERBOSE
    printf("..READY FOR VM\n");
#endif

    VM::VM vm;
    vm.init(f, argc, argv);

#if VERBOSE
    printf("..READY TO EXECUTE\n");
#endif
    vm.run();

    delete reader;
    //delete f;

    return 0;
}