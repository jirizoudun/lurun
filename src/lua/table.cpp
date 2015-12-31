
#include "../common.h"

namespace Lua {

    Table::Table() {
        hash_part = new std::unordered_map<ValueObject,ValueObject,ValueObjectHasher>();
    }
    Table::Table(const Table& other) {
        hash_part = new std::unordered_map<ValueObject,ValueObject,ValueObjectHasher>(*other.hash_part);
    }
    Table::~Table() {
        delete hash_part;
    }

    void Table::set(ValueObject key, ValueObject value) {
        (*hash_part)[key] = value;
    }
    void Table::set(const char* key, ValueObject value) { // TODO refactor?
        ValueObject valKey(LUA_TSTRING, ALLOC_STRING(key));
        hash_part->insert(std::make_pair<ValueObject&,ValueObject&>(valKey, value));
    }

    ValueObject Table::get(const char* key) const {
        StringObject* so = new StringObject(key); // temporary, no need for gc
        ValueObject ret = get(ValueObject(LUA_TSTRING, so));
        delete so;
        return ret;
    }

    ValueObject Table::get(ValueObject key) const {
        if (hash_part->find(key) != hash_part->end()) {
            return hash_part->at(key);
        } else {
            return ValueObject();
        }
    }
    std::pair<ValueObject,ValueObject> Table::next(ValueObject key) {

        std::unordered_map<ValueObject, ValueObject, ValueObjectHasher>::iterator it;
        if (IS_NIL(key)) {
            it = hash_part->begin();
        } else {
            it = hash_part->find(key);
            if (it != hash_part->end()) {++it;}
        }

        if (it != hash_part->end()) {
            return std::pair<ValueObject,ValueObject>(it->first, it->second);
        } else {
            return std::pair<ValueObject,ValueObject>(ValueObject(), ValueObject());
        }
    }

    void Table::setLen(int length) {
        len = length;
    }

    int Table::getLen() const {
        return len;
    }

    void Table::print() {
        printf("Table <%zu elements, mt: %i>\n", hash_part->size(), metatable != NULL);
#if DEBUG_TABLE_CONTENTS
        for (auto it = hash_part->begin(); it != hash_part->end(); ++it) {
            printf("\t");
            printf("[%i] ", (it->second).type);
            (it->first).print();
        }
#endif
    }

    void Table::gc() const {
        if (metatable != NULL) {
            VM::HeapManager::markGray((char*)metatable);
        }
        for (auto it = hash_part->begin(); it != hash_part->end(); ++it) {
            (it->first).gc();
            (it->second).gc();
        }
    }
}