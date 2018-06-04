#ifndef JAVACOMPILER_PHIINSTRUCTION_H
#define JAVACOMPILER_PHIINSTRUCTION_H

#include "instruction.h"
#include "opcode.h"

class PhiInstruction : public Instruction {
private:
    std::map<int, int> blockAddressToVariableNumber;
public:
    PhiInstruction() : Instruction(-1, op_phi) {}

    void addToPhiFunc(int blockAddress, int variableNumber) {
        blockAddressToVariableNumber[blockAddress] = variableNumber;
    }

    int getVariableNumber(int blockAddress) {
        return blockAddressToVariableNumber[blockAddress];
    }

    int getNumberOfVarsInPhiFunction() const {
        return blockAddressToVariableNumber.size();
    }

    std::map<int, int> &viewMap() {
        return blockAddressToVariableNumber;
    };
};

#endif //JAVACOMPILER_PHIINSTRUCTION_H
