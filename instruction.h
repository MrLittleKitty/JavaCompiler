#ifndef JAVACOMPILER_INSTRUCTION_H
#define JAVACOMPILER_INSTRUCTION_H

#include <vector>

class Instruction {
private:
    unsigned char opCode;
    std::vector<unsigned char> operands;
public:
    Instruction(unsigned char op_code, std::vector<unsigned char> operands)
            : opCode(op_code), operands(std::move(operands)) {

    }

    Instruction(unsigned char op_code)
            : opCode(op_code) {

    }

    int getOpCode() const {
        return opCode;
    }

    std::vector<unsigned char> &getOperands() {
        return operands;
    }
};

#endif //JAVACOMPILER_INSTRUCTION_H
