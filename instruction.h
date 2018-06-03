#ifndef JAVACOMPILER_INSTRUCTION_H
#define JAVACOMPILER_INSTRUCTION_H

#include <vector>

class Instruction {
private:
    int byteCodeIndex;
    unsigned char opCode;
    std::vector<unsigned char> operands;
public:
    Instruction(int byteCodeIndex, unsigned char op_code, std::vector<unsigned char> operands)
            : byteCodeIndex(byteCodeIndex), opCode(op_code), operands(std::move(operands)) {

    }

    Instruction(int byteCodeIndex, unsigned char op_code)
            : byteCodeIndex(byteCodeIndex), opCode(op_code) {

    }

    int getOpCode() const {
        return opCode;
    }

    std::vector<unsigned char> &getOperands() {
        return operands;
    }

    int getByteCodeIndex() const {
        return byteCodeIndex;
    }
};

#endif //JAVACOMPILER_INSTRUCTION_H
