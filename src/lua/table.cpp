//
// Created by Tomas on 17. 11. 2015.
//

#include "../common.h"

namespace Lua {

    Table::Table() {
    }

    void Table::set(ValueObject key, ValueObject value) {
        hash_part[key] = value;
    }

    ValueObject Table::get(ValueObject key) {
        return hash_part[key];
    }

    void Table::print() {
        printf("Table contents:\n");
        for (auto it = hash_part.begin(); it != hash_part.end(); ++it) {
            (it->first).print();
            printf("=> ");
            (it->second).print();
        }
    }

}