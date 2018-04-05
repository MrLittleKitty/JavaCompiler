#ifndef JAVACOMPILER_INSTRUCTION_H
#define JAVACOMPILER_INSTRUCTION_H

#include "operand.h"

class Instruction {
    private:
        int opCode;
        Operand operands[];
};
#endif //JAVACOMPILER_INSTRUCTION_H
