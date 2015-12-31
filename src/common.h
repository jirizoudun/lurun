
#ifndef LURUN_COMMON_H
#define LURUN_COMMON_H

#define VERBOSE 0
#define DEBUG 0
#define DEBUG_TABLE_CONTENTS 1
#define DEBUG_STACK 0
#define DEBUG_HEAP 0
#define DEBUG_HEAP_STATUS 0
#define DEBUG_HEAP_ALLOC 0

#define MAX_FILENAME_LENGTH 128
#define MAX_LINE_LENGTH 500
#define BUFFER_SIZE 1048576 //MB

#define INSTRUCTION_SIZE 4






/*
** size and position of opcode arguments.
*/
#define SIZE_C		9
#define SIZE_B		9
#define SIZE_Bx		(SIZE_C + SIZE_B)
#define SIZE_A		8
#define SIZE_Ax		(SIZE_C + SIZE_B + SIZE_A)

#define SIZE_OP		6

#define POS_OP		0
#define POS_A		(POS_OP + SIZE_OP)
#define POS_C		(POS_A + SIZE_A)
#define POS_B		(POS_C + SIZE_C)
#define POS_Bx		POS_C
#define POS_Ax		POS_A

#define MAXARG_Bx        ((1<<SIZE_Bx)-1)
#define MAXARG_sBx        (MAXARG_Bx>>1)         /* 'sBx' is signed */

/* creates a mask with 'n' 1 bits at position 'p' */
#define MASK1(n,p)	((~((~(unsigned int)0)<<(n)))<<(p))

#define getarg(i,pos,size)	(((int)((i)>>pos)) & MASK1(size,0))

#define GET_OPCODE(i)	((OpCode)((i>>POS_OP) & MASK1(SIZE_OP,0)))

#define GETARG_A(i)	getarg(i, POS_A, SIZE_A)
#define GETARG_B(i)	getarg(i, POS_B, SIZE_B)
#define GETARG_C(i)	getarg(i, POS_C, SIZE_C)
#define GETARG_Bx(i)	getarg(i, POS_Bx, SIZE_Bx)
#define GETARG_Ax(i)	getarg(i, POS_Ax, SIZE_Ax)
#define GETARG_sBx(i)	(GETARG_Bx(i)-MAXARG_sBx)







/*
** Macros to operate RK indices
*/

/* this bit 1 means constant (0 means register) */
#define BITRK		(1 << (SIZE_B - 1))

/* test whether value is a constant */
#define ISK(x)		((x) & BITRK)

/* gets the index of the constant */
#define INDEXK(r)	((int)(r) & ~BITRK)

#define MAXINDEXRK	(BITRK - 1)

/* code a constant index as a RK value */
#define RKASK(x)	((x) | BITRK)


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
#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include <assert.h>
#include "vm/gc_object.h"
#include "lua/lua.h"
#include "header.h"
#include "reader.h"
#include "vm/vm_common.h"
#include <math.h>

#endif //LURUN_COMMON_H
