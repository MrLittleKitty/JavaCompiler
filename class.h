#ifndef JAVACOMPILER_CLASS_H
#define JAVACOMPILER_CLASS_H

#include <utility>
#include <map>
#include "attribute.h"
#include "field.h"
#include "method.h"
#include "parserinfo.h"

class Class {
private:
    std::string name;
    std::vector<Method> methods;
    std::vector<Field> fields;
    std::vector<Attribute> attributes;
//    Class *superClass;
//    std::vector<Class> interfaces;

    std::map<int, Class_info *> classMap;
    std::map<int, Ref_info *> fieldRefMap;
    std::map<int, Ref_info *> methodRefMap;
    std::map<int, Ref_info *> interfaceMethodRefMap;
    std::map<int, String_info *> stringInfoMap;
    std::map<int, Number_info *> integerMap;
    std::map<int, Number_info *> floatMap;
    std::map<int, BigNumber_info *> longMap;
    std::map<int, BigNumber_info *> doubleMap;
    std::map<int, NameAndType_info *> nameAndTypeMap;
    std::map<int, std::string> stringMap;
public:
    Class(std::string name, std::vector<Method> methods, std::vector<Attribute> attributes,
          std::map<int, Class_info *> classMap1,
          std::map<int, Ref_info *> fieldRefMap1,
          std::map<int, Ref_info *> methodRefMap1,
          std::map<int, Ref_info *> interfaceMethodRefMap1,
          std::map<int, String_info *> stringInfoMap1,
          std::map<int, Number_info *> integerMap1,
          std::map<int, Number_info *> floatMap1,
          std::map<int, BigNumber_info *> longMap1,
          std::map<int, BigNumber_info *> doubleMap1,
          std::map<int, NameAndType_info *> nameAndTypeMap1,
          std::map<int, std::string> stringMap1)
            : name(name), methods(std::move(methods)), attributes(std::move(attributes)),
              classMap(std::move(classMap1)), fieldRefMap(std::move(fieldRefMap1)),
              methodRefMap(std::move(methodRefMap1)), interfaceMethodRefMap(std::move(interfaceMethodRefMap1)),
              stringInfoMap(std::move(stringInfoMap1)), integerMap(std::move(integerMap1)),
              floatMap(std::move(floatMap1)),
              longMap(std::move(longMap1)), doubleMap(std::move(doubleMap1)),
              nameAndTypeMap(std::move(nameAndTypeMap1)),
              stringMap(std::move(stringMap1)) {

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

    std::string getMethodNameFromConstantPool(int index) {
        return stringMap[nameAndTypeMap[methodRefMap[index]->name_and_type_index]->name_index];
    }
};

#endif //JAVACOMPILER_CLASS_H
