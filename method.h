#ifndef JAVACOMPILER_METHOD_H
#define JAVACOMPILER_METHOD_H

#include "qualifier.h"
#include "instruction.h"

class Method {
private:
    std::vector<Qualifier> qualifiers;
    std::vector<Instruction> instructions;
};

#endif //JAVACOMPILER_METHOD_H
