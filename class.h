#ifndef JAVACOMPILER_CLASS_H
#define JAVACOMPILER_CLASS_H

#include <utility>
#include "attribute.h"
#include "field.h"
#include "method.h"

class Class {
private:
    std::string name;
    std::vector<Method> methods;
    std::vector<Field> fields;
    std::vector<Attribute> attributes;
//    Class *superClass;
//    std::vector<Class> interfaces;
public:
    Class(std::string name, std::vector<Method> methods, std::vector<Attribute> attributes)
            : name(name), methods(std::move(methods)), attributes(std::move(attributes)) {

    }

    std::string getName() const {
        return name;
    }

    std::vector<Method> &getMethods() {
        return methods;
    }

    std::vector<Field> &getFields() {
        return fields;
    }

    std::vector<Attribute> &getAttributes() {
        return attributes;
    }
};

#endif //JAVACOMPILER_CLASS_H
