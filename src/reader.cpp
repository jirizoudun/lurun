//
// Created by Tomas on 15. 11. 2015.
//

#include "common.h"

using namespace Lua;

void Reader::skip(unsigned bytes) {
    cursor += bytes;
}

byte Reader::getNext() {
    return lfile.buffer[cursor++];
}

void Reader::readFile() {
    #if VERBOSE
    printf("Reading\n");
    #endif

    skip(HEADER_SIZE);
    (readFunction())->print();
}

Function* Reader::readFunction() {

    String*                 name        = readString(); // source name?
    int                     line_first  = readInt(); // line defined
    int                     line_last   = readInt(); // lastlinedefined
    byte                    num_params  = readByte(); // numparams
    byte                    is_vararg   = readByte(); // is_vararg
    byte                    max_stack   = readByte(); // maxstacksize
    InstructionList*        code        = readCode(); // code
    Container<ValueObject>  constants   = readConstants(); // constants

    /*
    readUpvalues();
    readProtos();
    readDebug();
     */

    return new Function(name, line_first, line_last, num_params, is_vararg, max_stack, code, constants);
}

byte Reader::readByte() {
    return Reader::getNext();
}
bool Reader::readBool() {
    return getNext();
}
double Reader::readNum() {
    // TODO implement
    return 0.;
}
int Reader::readInt() {
    int number;
    number = 0;
    for (int i=0; i<4; i++) { // TODO replace 4 with constant
        number |= (getNext() << (i*8)); // TODO better?
    }
    return number;
}
String* Reader::readString() {
    uchar byte = getNext();
    if (byte == 0) {
        return NULL;
    } else {
        size_t len = byte;
        // TODO read long string
        char* str = new char[len];
        memcpy(str, lfile.buffer+cursor, len-1);
        str[len-1] = '\0';
        cursor += len-1;
        return new String(len, str);
    }
}

InstructionList* Reader::readCode() {
    int count = readInt();
    Instruction** inst = new Instruction*[count];

    for (int i=0; i<count; i++) {
        inst[i] = new Instruction(readInt()); // TODO counting on instruction size == int size
    }
    return new InstructionList(count, inst);
}

Container<ValueObject> Reader::readConstants() {

    Container<ValueObject> values(readInt());

    for (int i=0; i<values.count; i++) {
        values[i].type = readByte();
        switch(values[i].type) {
            case LUA_TNIL:
                break;
            case LUA_TBOOLEAN:
                values[i].value.b = readBool();
                break;
            case LUA_TNUMFLT:
                values[i].value.d = readNum();
                break;
            case LUA_TNUMINT:
                values[i].value.i = readInt();
                break;
            case LUA_TSHRSTR:
            case LUA_TLNGSTR:
                values[i].value.p = (void*)readString();
                break;
            default:
                exit(0); // TODO error
        }
    }
    return values;
}
void Reader::readUpvalues() {
    int count = readInt();
    for (int i=0; i<count; i++) {
        byte instack = readByte();
        byte idx = readByte();
    }
}

void Reader::readProtos() {
    int count = readInt();
    for (int i=0; i<count; i++) {
        readFunction();
    }
}

void Reader::readDebug() {}
