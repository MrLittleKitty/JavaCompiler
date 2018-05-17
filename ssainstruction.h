#ifndef JAVACOMPILER_SSAINSTRUCTION_H
#define JAVACOMPILER_SSAINSTRUCTION_H

#include <string>
#include <vector>
#include "basicblock.h"

#define t_BinaryInstruction 1
#define t_ConditionInstruction 2
#define t_ConstantIntInstruction 3
#define t_PhiInstruction 4
#define t_CallInstruction 5
#define t_UnconditionalBranchInstruction 6
#define t_ConditionalBranchInstruction 7
#define t_ReturnInstruction 8
#define t_ParameterInstruction 9

class SSAInstruction {
private:
    int instructionType;
    std::vector<std::string> operands;
public:
    explicit SSAInstruction(int instructionType) : instructionType(instructionType) {

    }

    int getInstructionType() const {
        return instructionType;
    }

    std::vector<std::string> &getOperands() {
        return operands;
    }
};

class BinaryInstruction : public SSAInstruction {
public:
    explicit BinaryInstruction(int instructionType) : SSAInstruction(instructionType) {}

};

class ConditionInstruction : public SSAInstruction {
public:
    explicit ConditionInstruction(int instructionType) : SSAInstruction(instructionType) {}

};

class ConstantIntInstruction : public SSAInstruction {
public:
    explicit ConstantIntInstruction(int instructionType) : SSAInstruction(instructionType) {}

};

class PhiInstruction : public SSAInstruction {
private:
    std::vector<BasicBlock *> blocks;
    std::vector<SSAInstruction *> instructions;
public:
    explicit PhiInstruction(int instructionType) : SSAInstruction(instructionType) {}

};

class CallInstruction : public SSAInstruction {
public:
    explicit CallInstruction(int instructionType) : SSAInstruction(instructionType) {}

};

class UnconditionalBranchInstruction : public SSAInstruction {
public:
    explicit UnconditionalBranchInstruction(int instructionType) : SSAInstruction(instructionType) {}

};

class ConditionalBranchInstruction : public SSAInstruction {
public:
    explicit ConditionalBranchInstruction(int instructionType) : SSAInstruction(instructionType) {}

};

class ReturnInstruction : public SSAInstruction {
public:
    explicit ReturnInstruction(int instructionType) : SSAInstruction(instructionType) {}

};

class ParameterInstruction : public SSAInstruction {
public:
    explicit ParameterInstruction(int instructionType) : SSAInstruction(instructionType) {}

};

#endif //JAVACOMPILER_SSAINSTRUCTION_H
