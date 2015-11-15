//
// Created by Jiří Zoudun on 15/11/15.
//

#ifndef LUABYTECODELOADER_HEADER_H
#define LUABYTECODELOADER_HEADER_H

#define SIGNATURE_SIZE 4
#define VERSION_SIZE 1
#define FORMAT_SIZE 1
#define LUACDATA_SIZE 6
#define INTSIZE_SIZE 1
#define SIZETSIZE_SIZE 1
#define INSTRUCTIONSIZE_SIZE 1
#define LUAINTSIZE_SIZE 1
#define LUANUMBERSIZE_SIZE 1
#define LUACINT_SIZE 8
#define LUACNUM_SIZE 8
#define SIZEUPVALUE_SIZE 1

#define HEADER_SIZE 34

/**
 * Function that checks validity of header field TODO.
 *
 * @param TODO Char array that contains header field.
 * @param size Size of char array.
 */
bool checkheader_signature(unsigned char *signature, unsigned int size);

/**
 * Function that checks validity of header field TODO.
 *
 * @param TODO Char array that contains header field.
 * @param size Size of char array.
 */
bool checkheader_version(unsigned char *version, unsigned int size);

/**
 * Function that checks validity of header field TODO.
 *
 * @param TODO Char array that contains header field.
 * @param size Size of char array.
 */
bool checkheader_format(unsigned char *format, unsigned int size);

/**
 * Function that checks validity of header field TODO.
 *
 * @param TODO Char array that contains header field.
 * @param size Size of char array.
 */
bool checkheader_luacdata(unsigned char *luacdata, unsigned int size);

/**
 * Function that checks validity of header field TODO.
 *
 * @param TODO Char array that contains header field.
 * @param size Size of char array.
 */
bool checkheader_intsize(unsigned char *intsize, unsigned int size);

/**
 * Function that checks validity of header field TODO.
 *
 * @param TODO Char array that contains header field.
 * @param size Size of char array.
 */
bool checkheader_sizetsize(unsigned char *sizetsize, unsigned int size);

/**
 * Function that checks validity of header field TODO.
 *
 * @param TODO Char array that contains header field.
 * @param size Size of char array.
 */
bool checkheader_instructionsize(unsigned char *instructionsize, unsigned int size);

/**
 * Function that checks validity of header field TODO.
 *
 * @param TODO Char array that contains header field.
 * @param size Size of char array.
 */
bool checkheader_luaintsize(unsigned char *luaintsize, unsigned int size);

/**
 * Function that checks validity of header field TODO.
 *
 * @param TODO Char array that contains header field.
 * @param size Size of char array.
 */
bool checkheader_luanumbersize(unsigned char *luanumbersize, unsigned int size);

/**
 * Function that checks validity of header field TODO.
 *
 * @param TODO Char array that contains header field.
 * @param size Size of char array.
 */
bool checkheader_luacint(unsigned char *luacint, unsigned int size);

/**
 * Function that checks validity of header field TODO.
 *
 * @param TODO Char array that contains header field.
 * @param size Size of char array.
 */
bool checkheader_luacnum(unsigned char *luacnum, unsigned int size);

/**
 * Function that checks validity of header field TODO.
 *
 * @param TODO Char array that contains header field.
 * @param size Size of char array.
 */
bool checkheader_sizeupvalue(unsigned char *sizeupvalue, unsigned int size);


#endif //LUABYTECODELOADER_HEADER_H
