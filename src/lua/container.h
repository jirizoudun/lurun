//
// Created by Tomas on 15. 11. 2015.
//

#ifndef LURUN_CONTAINER_H
#define LURUN_CONTAINER_H

namespace Lua {

    template<class T>
    class Container {
    public:
        int count;
        T* content;

        Container(int count): count(count) {
            content = new T[count];
        };

        T& operator[](size_t idx) {
            return content[idx];
        };

        void print() {
            for (int i=0; i<count; i++) {
                printf("\t");
                content[i].print();
            }
        }
    };

}

#endif //LURUN_CONTAINER_H
