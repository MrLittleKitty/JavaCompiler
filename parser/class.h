#ifndef JAVACOMPILER_CLASS_H
#define JAVACOMPILER_CLASS_H

#include "attribute.h"
#include "field.h"
#include "method.h"

class Class {
    private:
        Method methods[];
        Field fields[];
        Attribute attributes[];
        Class superclasses[];
};
#endif //JAVACOMPILER_CLASS_H
