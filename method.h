#ifndef JAVACOMPILER_METHOD_H
#define JAVACOMPILER_METHOD_H

#include "qualifier.h"
#include "instruction.h"
#include "code.h"

class Method {
private:
    std::string name;
    Code *byteCode;
//    std::vector<std::string> qualifiers;
//    std::vector<Instruction> instructions;
public:
    Method(std::string name, Code *byteCode)
            : name(name), byteCode(byteCode) {

    }
};

#endif //JAVACOMPILER_METHOD_H
