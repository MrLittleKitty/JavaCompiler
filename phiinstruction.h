#ifndef JAVACOMPILER_PHIINSTRUCTION_H
#define JAVACOMPILER_PHIINSTRUCTION_H

#include "instruction.h"
#include "opcode.h"

class PhiInstruction : public Instruction {
private:
    int variableNumber;
public:
    PhiInstruction(int variableNumber) : Instruction(-1, op_phi), variableNumber(variableNumber) {}

    int getVariableNumber() const {
        return variableNumber;
    }
};

#endif //JAVACOMPILER_PHIINSTRUCTION_H
