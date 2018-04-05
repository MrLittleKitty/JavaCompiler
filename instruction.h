#ifndef JAVACOMPILER_INSTRUCTION_H
#define JAVACOMPILER_INSTRUCTION_H

#include <vector>
#include "operand.h"

class Instruction {
private:
    int opCode;
    std::vector<Operand> operands;
};

#endif //JAVACOMPILER_INSTRUCTION_H
