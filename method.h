#ifndef JAVACOMPILER_METHOD_H
#define JAVACOMPILER_METHOD_H

#include <string>
#include <utility>
#include "instruction.h"
#include "code.h"
#include "opcode.h"
#include "basicblock.h"

class Method {
private:
    std::string name;
    std::string access;
    bool is_static;

    std::string descriptor;
    std::string returnType;
    std::vector<std::string> parameterTypes;

    Code *code;
    std::vector<Instruction> instructions;
    std::vector<BasicBlock *> basicBlocks;

    static short constructOffset(unsigned char one, unsigned char two) {
        return (short) (((unsigned short) (one << 8)) | two);
    }

    static int getIndexOfByteCode(std::vector<Instruction> &instructions, int byteCodeIndex) {
        for (int i = 0; i < instructions.size(); i++) {
            if (instructions[i].getByteCodeIndex() == byteCodeIndex)
                return i;
        }
        return -1;
    }

    void parseDescriptor(std::string descriptor) {

        int index = 0;
        while (true) {
            if (descriptor[index] == '(' || descriptor[index] == ' ') {
                index++;
                continue;
            }
            if (descriptor[index] == ')') {
                index++;
                break;
            }

            std::string val = parseFieldType(descriptor, index);
            parameterTypes.push_back(val);
        }

        returnType = parseFieldType(descriptor, index);
    }

    std::string parseFieldType(std::string &descriptor, int &index) {
        if (descriptor[index] == 'B' ||
            descriptor[index] == 'C' ||
            descriptor[index] == 'D' ||
            descriptor[index] == 'F' ||
            descriptor[index] == 'I' ||
            descriptor[index] == 'J' ||
            descriptor[index] == 'S' ||
            descriptor[index] == 'Z' ||
            descriptor[index] == 'V') {
            index++;
            return std::string(1, descriptor[index - 1]);
        }

        if (descriptor[index] == 'L') {
            index++;
            int startIndex = index;

            while (descriptor[index] != ';')
                index++;

            index++;
            return "L" + descriptor.substr(startIndex, index - startIndex - 1) + ";";
        }

        if (descriptor[index] == '[') {
            index++;
            return "[" + parseFieldType(descriptor, index);
        }
    }

    void parseByteCode(unsigned char *byteCode, int length) {
        for (int i = 0; i < length; i++) {
            unsigned char opCode = byteCode[i];
            switch (opCode) {
                case op_iconst_ml:
                case op_iconst_0:
                case op_iconst_1:
                case op_iconst_2:
                case op_iconst_3:
                case op_iconst_4:
                case op_iconst_5:
                case op_return:
                case op_iadd:
                case op_aload_0:
                case iload_0:
                case iload_1:
                case iload_2:
                case iload_3:
                case istore_0:
                case istore_1:
                case istore_2:
                case istore_3: {
                    instructions.emplace_back(Instruction(i, opCode));
                    break;
                }

                case op_iload:
                case op_istore:
                case op_bipush: {
                    int codeIndex = i;
                    i++;
                    unsigned char index = byteCode[i];
                    std::vector<unsigned char> operands;
                    operands.emplace_back(index);
                    instructions.emplace_back(Instruction(codeIndex, opCode, operands));
                    break;
                }

                case op_if_icmpne:
                case op_if_icmpeq:
                case op_if_icmpgt:
                case op_if_icmplt:
                case op_if_icmpge:
                case op_if_icmple:
                case op_ifeq:
                case op_ifne:
                case op_ifgt:
                case op_iflt:
                case op_ifle:
                case op_ifge:
                case op_goto:
                case op_invokestatic:
                case op_invokespecial:
                case op_getstatic:
                case op_invokevirtual:
                case op_iinc: {
                    int codeIndex = i;
                    i++;
                    unsigned char branch1 = byteCode[i];
                    i++;
                    unsigned char branch2 = byteCode[i];
                    std::vector<unsigned char> operands;
                    operands.emplace_back(branch1);
                    operands.emplace_back(branch2);
                    instructions.emplace_back(Instruction(codeIndex, opCode, operands));
                    break;
                }
                default: {
                    printf("Unknown opCode: %x \n", opCode);
                }
            }
        }
    }

    BasicBlock *findBasicBlockWithAddress(int address) {
        for (auto block : basicBlocks) {
            if (block->getStartingAddress() == address)
                return block;
        }
        return nullptr;
    }

    void buildBasicBlocks(int instructionIndex) {
        BasicBlock *current = nullptr;
        for (; instructionIndex < instructions.size(); instructionIndex++) {
            Instruction instruction = instructions[instructionIndex];
            if (current == nullptr)
                current = new BasicBlock(instruction.getByteCodeIndex());

            current->addInstruction(instruction);
            switch (instruction.getOpCode()) {
//                case op_return:
//                case op_invokevirtual:
//                case op_invokestatic:
//                case op_invokespecial:
                case op_goto: {
                    basicBlocks.emplace_back(current);
                    current = nullptr;
                    int jumpToAddress = instruction.getByteCodeIndex() +
                                        constructOffset(instruction.getOperands()[0], instruction.getOperands()[1]);
                    if (findBasicBlockWithAddress(jumpToAddress) == nullptr) {
                        buildBasicBlocks(getIndexOfByteCode(instructions, jumpToAddress));
                    }
                    return;
                }
                case op_if_icmpeq:
                case op_if_icmpne:
                case op_if_icmplt:
                case op_if_icmpge:
                case op_if_icmpgt:
                case op_if_icmple:
                case op_ifeq:
                case op_ifne:
                case op_iflt:
                case op_ifge:
                case op_ifgt:
                case op_ifle: {
                    basicBlocks.emplace_back(current);
                    current = nullptr;

                    int jumpToAddress = instruction.getByteCodeIndex() +
                                        constructOffset(instruction.getOperands()[0], instruction.getOperands()[1]);
                    if (findBasicBlockWithAddress(jumpToAddress) == nullptr) {
                        buildBasicBlocks(getIndexOfByteCode(instructions, jumpToAddress));
                    }

                    int index = instructionIndex + 1;
                    if (index < instructions.size()) {
                        Instruction next = instructions[index];
                        int fallThroughAddress = next.getByteCodeIndex();
                        if (findBasicBlockWithAddress(fallThroughAddress) == nullptr) {
                            buildBasicBlocks(getIndexOfByteCode(instructions, fallThroughAddress));
                        }
                    }
                    return;
                }
                default: {
                    //Do nothing unless it is a control flow instruction
                    break;
                }
            }
        }

        if (current != nullptr)
            basicBlocks.emplace_back(current);
    }

    void linkBasicBlocks() {
        for (int i = 0; i < basicBlocks.size(); i++) {
            BasicBlock *block = basicBlocks[i];
            Instruction instruction = block->getInstructions().back();
            switch (instruction.getOpCode()) {
                case op_goto: {
                    int jumpToAddress = instruction.getByteCodeIndex() +
                                        constructOffset(instruction.getOperands()[0], instruction.getOperands()[1]);
                    BasicBlock *successor = findBasicBlockWithAddress(jumpToAddress);
                    if (successor == nullptr)
                        printf("Attempted to find a basic block with address that isn't valid from goto");
                    else {
                        successor->addPredecessor(block);
                        block->addSuccessor(successor);
                    }
                    break;
                }
//                case op_invokevirtual:
//                case op_invokestatic:
//                case op_invokespecial:
                case op_if_icmpeq:
                case op_if_icmpne:
                case op_if_icmplt:
                case op_if_icmpge:
                case op_if_icmpgt:
                case op_if_icmple:
                case op_ifeq:
                case op_ifne:
                case op_iflt:
                case op_ifge:
                case op_ifgt:
                case op_ifle: {
                    int jumpToAddress = instruction.getByteCodeIndex() +
                                        constructOffset(instruction.getOperands()[0], instruction.getOperands()[1]);
                    BasicBlock *successor1 = findBasicBlockWithAddress(jumpToAddress);
                    if (successor1 == nullptr)
                        printf("Attempted to find a basic block with address that isn't valid for jump");
                    else {
                        successor1->addPredecessor(block);
                        block->addSuccessor(successor1);
                    }

                    int index = getIndexOfByteCode(instructions, instruction.getByteCodeIndex()) + 1;
                    if (index < instructions.size()) {
                        BasicBlock *successor2 = findBasicBlockWithAddress(instructions[index].getByteCodeIndex());
                        if (successor2 == nullptr)
                            printf("Attempted to find a basic block with address that isn't valid for fallthrough");
                        else {
                            successor2->addPredecessor(block);
                            block->addSuccessor(successor2);
                        }
                    }
                    break;
                }
                default: {
                    break;
                }
            }
        }
    }

public:
    Method(std::string name, std::string descriptor, Code *byteCode, std::vector<std::string> qualifiers)
            : name(name), descriptor(descriptor), code(byteCode) {

        //Methods are default public and non-static
        access = "public";
        is_static = false;

        //Go through the qualifiers and find the access type and if this method is static
        for (auto qualifier : qualifiers) {
            if (qualifier == "public" || qualifier == "private" || qualifier == "protected")
                access = qualifier;
            else if (qualifier == "static")
                is_static = true;
        }

        //Parse the descriptor to get type information

        parseDescriptor(descriptor);

        //Parse the bytecode into instruction objects
        parseByteCode(code->getByteCode(), code->getSize());

        //Build basic blocks from the instruction objects
        buildBasicBlocks(0);

        //Link the basic blocks together
        linkBasicBlocks();
    }

    std::string getName() const {
        return name;
    }

    bool isStatic() const {
        return is_static;
    }

    std::string getAccess() const {
        return access;
    }

    Code *getCode() const {
        return code;
    }

    std::string getDescriptor() const {
        return descriptor;
    }

    std::vector<Instruction> &getInstructions() {
        return instructions;
    }
};

#endif //JAVACOMPILER_METHOD_H
