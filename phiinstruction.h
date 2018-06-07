#ifndef JAVACOMPILER_PHIINSTRUCTION_H
#define JAVACOMPILER_PHIINSTRUCTION_H

#include "instruction.h"
#include "opcode.h"

class PhiInstruction : public Instruction {
private:
    int originalLHS;
    int lhsVariable;
    std::map<int, int> *rhsVariableMap;

    Instruction *createStoreInstruction(int varNumber, int bytecodeIndex) {
        if (varNumber == 0)
            return new Instruction(bytecodeIndex, op_istore_0);
        else if (varNumber == 1)
            return new Instruction(bytecodeIndex, op_istore_1);
        else if (varNumber == 2)
            return new Instruction(bytecodeIndex, op_istore_2);
        else if (varNumber == 3)
            return new Instruction(bytecodeIndex, op_istore_3);
        else {
            std::vector<unsigned char> operands;
            operands.emplace_back((unsigned char) varNumber);
            return new Instruction(bytecodeIndex, op_istore, operands);
        }
    }

    Instruction *createLoadInstruction(int varNumber, int bytecodeIndex) {
        if (varNumber == 0)
            return new Instruction(bytecodeIndex, op_iload_0);
        else if (varNumber == 1)
            return new Instruction(bytecodeIndex, op_iload_1);
        else if (varNumber == 2)
            return new Instruction(bytecodeIndex, op_iload_2);
        else if (varNumber == 3)
            return new Instruction(bytecodeIndex, op_iload_3);
        else {
            std::vector<unsigned char> operands;
            operands.emplace_back((unsigned char) varNumber);
            return new Instruction(bytecodeIndex, op_iload, operands);
        }
    }

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

    std::vector<Instruction *> getInstructions(int fromBlock) {
        std::vector<Instruction *> result;
        if (rhsVariableMap->count(fromBlock) > 0) {
            result.emplace_back(createLoadInstruction((*rhsVariableMap)[fromBlock], getByteCodeIndex()));
            result.emplace_back(createStoreInstruction(getCurrentLHS(), getByteCodeIndex()));
        }
        return result;
    }
};

#endif //JAVACOMPILER_PHIINSTRUCTION_H
