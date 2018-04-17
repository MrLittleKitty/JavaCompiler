#ifndef JAVACOMPILER_METHOD_H
#define JAVACOMPILER_METHOD_H

#include <string>
#include <utility>
#include "instruction.h"
#include "code.h"

class Method {
private:
    std::string name;
    std::string access;
    bool is_static;

    std::string returnType;
    std::vector<std::string> parameterTypes;

    Code *byteCode;

//    std::vector<Instruction> instructions;

    void parseDescriptor(std::string descriptor) {

        int index = 0;
        while (true) {
            if (descriptor[index] == '(' || descriptor[index] == ' ') {
                index++;
                continue;
            }
            if (descriptor[index] == ')') {
                index++;
                break;
            }

            std::string val = parseFieldType(descriptor, index);
            parameterTypes.push_back(val);
        }

        returnType = parseFieldType(descriptor, index);
    }

    std::string parseFieldType(std::string &descriptor, int &index) {
        if (descriptor[index] == 'B' ||
            descriptor[index] == 'C' ||
            descriptor[index] == 'D' ||
            descriptor[index] == 'F' ||
            descriptor[index] == 'I' ||
            descriptor[index] == 'J' ||
            descriptor[index] == 'S' ||
            descriptor[index] == 'Z' ||
            descriptor[index] == 'V') {
            index++;
            return std::string(1, descriptor[index - 1]);
        }

        if (descriptor[index] == 'L') {
            index++;
            int startIndex = index;

            while (descriptor[index] != ';')
                index++;

            index++;
            return "L" + descriptor.substr(startIndex, index - startIndex - 1) + ";";
        }

        if (descriptor[index] == '[') {
            index++;
            return "[" + parseFieldType(descriptor, index);
        }
    }

public:
    Method(std::string name, std::string descriptor, Code *byteCode, std::vector<std::string> qualifiers)
            : name(name), byteCode(byteCode) {

        //Methods are default public and non-static
        access = "public";
        is_static = false;

        //Go through the qualifiers and find the access type and if this method is static
        for (auto qualifier : qualifiers) {
            if (qualifier == "public" || qualifier == "private" || qualifier == "protected")
                access = qualifier;
            else if (qualifier == "static")
                is_static = true;
        }

        //Parse the descriptor to get type information
        parseDescriptor(descriptor);

    }

    bool isStatic() const {
        return is_static;
    }

    std::string getAccess() const {
        return access;
    }
};

#endif //JAVACOMPILER_METHOD_H
