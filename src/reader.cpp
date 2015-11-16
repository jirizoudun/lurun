//
// Created by Tomas on 15. 11. 2015.
//

#include "common.h"

using namespace Lua;

void Reader::skip(unsigned bytes) {
    cursor += bytes;
}
void Reader::copy(void* dest, size_t count) {
    memcpy(dest, lfile.buffer+cursor, count);
    cursor += count;
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
    Container<ValueObject>* constants   = readConstants(); // constants
    Container<Upvalue>*     upvalues    = readUpvalues(); // upvalues
    std::vector<Function*>* protos      = readProtos(); // protos
    readDebug();

    return new Function(name, line_first, line_last, num_params, is_vararg, max_stack, code, constants, upvalues, protos);
}

byte Reader::readByte() {
    return Reader::getNext();
}
bool Reader::readBool() {
    return getNext();
}
int Reader::readInt() {
    int number;
    copy((void*)&number, sizeof(number));
    return number;
}
double Reader::readNumber() {
    double number;
    copy((void*)&number, sizeof(number)); // TODO shouldn't count on double being 8 bytes
    return number;
}
long long Reader::readInteger() {
    long long number;
    copy((void*)&number, sizeof(number)); // TODO shouldn't count on long long being 8 bytes
    return number;
}

String* Reader::readString() {
    byte size = readByte();
    if (size == 0) {
        return NULL;
    } else {
        long long len;
        char* str;

        if (size == 0xFF) { // long string
            len = readInteger();
            str = new char[len];
            for (long long i=0; i<len-1; i++) {
                str[i] = getNext();
            }
        } else { // short string
            len = size;
            str = new char[len];
            memcpy(str, lfile.buffer+cursor, (size_t)len-1);
            cursor += len-1;
        }
        str[len-1] = '\0';

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

Container<ValueObject>* Reader::readConstants() {

    Container<ValueObject>* cont = new Container<ValueObject>(readInt());
    Container<ValueObject>& values = (*cont);

    for (int i=0; i<values.count; i++) {
        Container<ValueObject>& val = values;
        values[i].type = readByte();
        switch(values[i].type) {
            case LUA_TNIL:
                break;
            case LUA_TBOOLEAN:
                values[i].value.b = readBool();
                break;
            case LUA_TNUMFLT:
                values[i].value.d = readNumber();
                break;
            case LUA_TNUMINT:
                values[i].value.i = readInteger();
                break;
            case LUA_TSHRSTR:
            case LUA_TLNGSTR:
                values[i].value.p = (void*)readString();
                break;
            default:
                exit(1); // TODO error
        }
    }
    return cont;
}
Container<Upvalue>* Reader::readUpvalues() {

    Container<Upvalue>* cont = new Container<Upvalue>(readInt());

    for (int i=0; i<cont->count; i++) {
        (*cont)[i].instack = readByte();
        (*cont)[i].idx     = readByte();
    }

    return cont;
}

std::vector<Function*>* Reader::readProtos() {

    std::vector<Function*>* protos = new std::vector<Function*>();

    int count = readInt();
    for (int i=0; i<count; i++) {
        protos->push_back(readFunction());
    }
    return protos;
}

void Reader::readDebug() {
    int cnt = readInt();
    for (int i=0; i<cnt; i++) {
        readInt();
    }

    cnt = readInt();
    for (int i=0; i<cnt; i++) {
        readString();
        readInt();
        readInt();
    }

    cnt = readInt();
    for (int i=0; i<cnt; i++) {
        readString();
    }
}
