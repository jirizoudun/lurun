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
    void readFile();

private:

    byte                getNext();
    void                skip(unsigned bytes);

    byte                readByte();
    int                 readInt();

    bool                readBool();
    double              readNum();

    Lua::String*        readString();
    Lua::InstructionList* readCode();

    Lua::Function*      readFunction();
    Lua::Container<Lua::ValueObject>* readConstants();
    void readUpvalues();
    void readProtos();
    void readDebug();
};

#endif //LURUN_READER_H
