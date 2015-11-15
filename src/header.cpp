//
// Created by Jiří Zoudun on 15/11/15.
//

#include "common.h"
#include "header.h"

#include <stdio.h>

//!Valid header fields definitions.
uchar VALID_SIGNATURE[SIGNATURE_SIZE] = {(uchar)0x1b, (uchar)0x4c, (uchar)0x75, (uchar)0x61};
uchar VALID_VERSION[VERSION_SIZE] = {(uchar)0x53};
uchar VALID_FORMAT[FORMAT_SIZE] = {(uchar)0x00};
uchar VALID_LUACDATA[LUACDATA_SIZE] = {(uchar)0x19, (uchar)0x93, (uchar)0x0d, (uchar)0x0a, (uchar)0x1a, (uchar)0x0a};
uchar VALID_INTSIZE[INTSIZE_SIZE] = {(uchar)0x04};
uchar VALID_SIZETSIZE[SIZETSIZE_SIZE] = {(uchar)0x08};
uchar VALID_INSTRUCTIONSIZE[INSTRUCTIONSIZE_SIZE] = {(uchar)0x04};
uchar VALID_LUAINTSIZE[LUAINTSIZE_SIZE] = {(uchar)0x08};
uchar VALID_LUANUMBERSIZE[LUANUMBERSIZE_SIZE] = {(uchar)0x08};
uchar VALID_LUACINT[LUACINT_SIZE] = {(uchar)0x78, (uchar)0x56, (uchar)0x00, (uchar)0x00, (uchar)0x00, (uchar)0x00, (uchar)0x00, (uchar)0x00};
uchar VALID_LUACNUM[LUACNUM_SIZE] = {(uchar)0x00, (uchar)0x00, (uchar)0x00, (uchar)0x00, (uchar)0x00, (uchar)0x28, (uchar)0x77, (uchar)0x40};
uchar VALID_SIZEUPVALUE[SIZEUPVALUE_SIZE] = {(uchar)0x01};



bool testheaderfield(uchar *field, unsigned int field_size, const uchar *valid_field, unsigned int valid_field_size)
{
    int i = 0;
    for (; i < valid_field_size && i < field_size; i++) {
        if (field[i] != valid_field[i]) {
            return false;
        }
    }

    return true;
}

bool checkheader_signature(uchar *signature, unsigned int size) {
    return testheaderfield(signature, size, (const uchar*)VALID_SIGNATURE, sizeof(VALID_SIGNATURE));
}

bool checkheader_version(uchar *version, unsigned int size) {
    return testheaderfield(version, size, (const uchar*)VALID_VERSION, sizeof(VALID_VERSION));
}

bool checkheader_format(uchar *format, unsigned int size) {
    return testheaderfield(format, size, (const uchar*)VALID_FORMAT, sizeof(VALID_FORMAT));
}

bool checkheader_luacdata(uchar *luacdata, unsigned int size) {
    return testheaderfield(luacdata, size, (const uchar*)VALID_LUACDATA, sizeof(VALID_LUACDATA));
}

bool checkheader_intsize(uchar *intsize, unsigned int size) {
    return testheaderfield(intsize, size, (const uchar*)VALID_INTSIZE, sizeof(VALID_INTSIZE));
}

bool checkheader_sizetsize(uchar *sizetsize, unsigned int size) {
    return testheaderfield(sizetsize, size, (const uchar*)VALID_SIZETSIZE, sizeof(VALID_SIZETSIZE));
}

bool checkheader_instructionsize(uchar *instructionsize, unsigned int size) {
    return testheaderfield(instructionsize, size, (const uchar*)VALID_INSTRUCTIONSIZE, sizeof(VALID_INSTRUCTIONSIZE));
}

bool checkheader_luaintsize(uchar *luaintsize, unsigned int size) {
    return testheaderfield(luaintsize, size, (const uchar*)VALID_LUAINTSIZE, sizeof(VALID_LUAINTSIZE));
}

bool checkheader_luanumbersize(uchar *luanumbersize, unsigned int size) {
    return testheaderfield(luanumbersize, size, (const uchar*)VALID_LUANUMBERSIZE, sizeof(VALID_LUANUMBERSIZE));
}

bool checkheader_luacint(uchar *luacint, unsigned int size) {
    return testheaderfield(luacint, size, (const uchar*)VALID_LUACINT, sizeof(VALID_LUACINT));
}

bool checkheader_luacnum(uchar *luacnum, unsigned int size) {
    return testheaderfield(luacnum, size, (const uchar*)VALID_LUACNUM, sizeof(VALID_LUACNUM));
}

bool checkheader_sizeupvalue(uchar *sizeupvalue, unsigned int size) {
    return testheaderfield(sizeupvalue, size, (const uchar*)VALID_SIZEUPVALUE, sizeof(VALID_SIZEUPVALUE));
}
