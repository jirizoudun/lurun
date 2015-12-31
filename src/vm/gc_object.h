
#ifndef LURUN_GC_OBJECT_H
#define LURUN_GC_OBJECT_H


class GCObject {
public:
    virtual void gc() const =0;
};

#endif //LURUN_GC_OBJECT_H
