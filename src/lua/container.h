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


        Container(Container const& other) {
            exit(1); // FIXME not implemented
        }

        ~Container() {
            delete [] content;
        }

        T& operator[](size_t idx) {
            return content[idx];
        };

        void print() {
            for (int i=0; i<count; i++) {
                printf("\t");
                content[i].print();
            }
        }

        T get(const int idx) const {
            return content[idx];
        }
    };

}

#endif //LURUN_CONTAINER_H
