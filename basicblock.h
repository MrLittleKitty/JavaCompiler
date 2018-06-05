#ifndef JAVACOMPILER_BASICBLOCK_H
#define JAVACOMPILER_BASICBLOCK_H

#include <utility>
#include <vector>
#include "instruction.h"

class BasicBlock {
private:
    int startingAddress;
    std::vector<BasicBlock *> successors;
    std::vector<BasicBlock *> predecessors;
    std::vector<Instruction *> instructions;
public:
    BasicBlock(int startingAddress)
            : startingAddress(startingAddress) {
    }

    void addInstruction(Instruction *instruction) {
        instructions.emplace_back(instruction);
    }

    void addSuccessor(BasicBlock *block) {
        successors.emplace_back(block);
    }

    void addPredecessor(BasicBlock *block) {
        predecessors.emplace_back(block);
    }

    int getStartingAddress() const {
        return startingAddress;
    }

    std::vector<Instruction *> &getInstructions() {
        return instructions;
    }

    std::vector<BasicBlock *> &getPredecessors() {
        return predecessors;
    }

    std::vector<BasicBlock *> &getSuccessors() {
        return successors;
    }
};

#endif //JAVACOMPILER_BASICBLOCK_H
