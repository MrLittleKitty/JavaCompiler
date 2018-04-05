#ifndef JAVACOMPILER_CLASS_H
#define JAVACOMPILER_CLASS_H

#include "attribute.h"
#include "field.h"
#include "method.h"

class Class {
private:
    std::vector<Method> methods;
    std::vector<Field> fields;
    std::vector<Attribute> attributes;
    Class superClass;
    std::vector<Class> interfaces;
};

#endif //JAVACOMPILER_CLASS_H
