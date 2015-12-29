
#ifndef LURUN_TABLE_H
#define LURUN_TABLE_H

namespace Lua {

    // TODO use array part
    class Table {

        std::unordered_map<ValueObject,ValueObject,ValueObjectHasher> hash_part;
        int len = 0;

    public:

        Table* metatable = NULL;

        Table();
        Table(const Table& other);

        void set(ValueObject key, ValueObject value);
        void set(const char * key, ValueObject value);

        ValueObject get(const ValueObject key) const;
        std::pair<ValueObject,ValueObject> next(const ValueObject key);

        void setLen(int);
        int getLen() const;

        void print();
    };
}

#endif //LURUN_TABLE_H
