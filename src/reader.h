//
// Created by Tomas on 15. 11. 2015.
//

#ifndef LURUN_READER_H
#define LURUN_READER_H

class Reader {

public:
    LFILE lfile;
    unsigned cursor;

    Reader(LFILE &lfile): lfile(lfile), cursor(0) {};
    Lua::Function* readFile();

private:

    byte                getNext();
    void                skip(unsigned bytes);
    void                copy(void* dst, size_t count);

    byte                readByte();
    int                 readInt();

    bool                readBool();
    double              readNumber();
    long long           readInteger();

    Lua::StringObject *        readString();
    Lua::InstructionList* readCode();

    Lua::Function*      readFunction();

    Lua::Container<Lua::ValueObject>* readConstants();
    Lua::Container<Lua::UpvalueDesc>*     readUpvalues();
    std::vector<Lua::Function*>*      readProtos();
    void readDebug();
};

#endif //LURUN_READER_H
