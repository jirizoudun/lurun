
#ifndef LURUN_TABLE_H
#define LURUN_TABLE_H

namespace Lua {

    // TODO use array part
    class Table {

        std::unordered_map<ValueObject,ValueObject,ValueObjectHasher> hash_part;
        //ValueObject* array_part;

    public:

        Table();
        Table(const Table& other);

        void set(ValueObject key, ValueObject value);
        void set(const char * key, ValueObject value);

        ValueObject get(ValueObject key);

        void print();
    };
}

#endif //LURUN_TABLE_H
