//
// Created by Tomas on 17. 11. 2015.
//

#include "../common.h"

namespace Lua {

    Table::Table() {
    }

    Table::Table(const Table& other): hash_part(other.hash_part)
    {}

    void Table::set(ValueObject key, ValueObject value) {
        hash_part[key] = value;
    }
    void Table::set(const char* key, ValueObject value) { // TODO refactor?
        StringObject * str = new StringObject(key);
        ValueObject valKey(LUA_TSTRING, (void*)str);

        hash_part.insert(std::make_pair<ValueObject&,ValueObject&>(valKey, value));
    }

    ValueObject Table::get(ValueObject key) const {
        return hash_part.at(key);
    }

    void Table::print() {
        printf("Table <%zu elements>:\n", hash_part.size());
        for (auto it = hash_part.begin(); it != hash_part.end(); ++it) {
            printf("\t");
            printf("[%i] ", (it->second).type);
            (it->first).print();
        }
    }

}