#ifndef JAVACOMPILER_PHIINSTRUCTION_H
#define JAVACOMPILER_PHIINSTRUCTION_H

#include "instruction.h"
#include "opcode.h"

class PhiInstruction : public Instruction {
private:
    int lhsVariable;

public:
    PhiInstruction(int variable) : Instruction(-1, op_phi), lhsVariable(variable) {}

    int getLHS() const {
        return lhsVariable;
    }

    void setLHS(int LHS) {
        lhsVariable = LHS;
    }
};

#endif //JAVACOMPILER_PHIINSTRUCTION_H
