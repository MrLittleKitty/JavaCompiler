#ifndef JAVACOMPILER_PHIINSTRUCTION_H
#define JAVACOMPILER_PHIINSTRUCTION_H

#include "instruction.h"
#include "opcode.h"

class PhiInstruction : public Instruction {
private:
    int originalLHS;
    int lhsVariable;
    std::map<int, int> *rhsVariableMap;

public:
    PhiInstruction(int variable) : Instruction(-1, op_phi), lhsVariable(variable), originalLHS(variable) {
        rhsVariableMap = new std::map<int, int>();
    }

    ~ PhiInstruction() {
        delete rhsVariableMap;
    }

    int getOriginalLHS() const {
        return originalLHS;
    }

    int getCurrentLHS() const {
        return lhsVariable;
    }

    void setCurrentLHS(int LHS) {
        lhsVariable = LHS;
    }

    void setRHSVariable(int incomingBlockAddress, int value) {
        (*rhsVariableMap)[incomingBlockAddress] = value;
    }

    int getRHSVariable(int incomingBlockAddress) {
        return (*rhsVariableMap)[incomingBlockAddress];
    }

    std::map<int, int> *viewRHSMap() {
        return rhsVariableMap;
    };
};

#endif //JAVACOMPILER_PHIINSTRUCTION_H
