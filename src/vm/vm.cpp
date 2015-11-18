
#include "../common.h"

// create table
// create closure for initial chunk
// execute chunk

namespace VM {

    using namespace Lua;

    void VM::init() {
        Table table;

        /*
        Lua::ValueObject a(102);
        Lua::ValueObject b(105);
        Lua::ValueObject c(9998118861516);
        Lua::ValueObject d(161516);


        char str[] = "abcasafawfaw";
        Lua::String* s = new Lua::String(strlen(str), str);

        (*s).print();

        Lua::ValueObject s1;
        s1.type = LUA_TSHRSTR;
        s1.value.p = s;


        table.set(a, b);
        table.set(c, d);
        table.print();
        table.set(a, c);
        table.print();

        table.set(a, s1);
        table.print();

        table.set(s1, a);
        table.print();
        */
    }

    void VM::execute() {

    }

}
