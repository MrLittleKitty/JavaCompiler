#ifndef JAVACOMPILER_METHOD_H
#define JAVACOMPILER_METHOD_H

#include <string>
#include <utility>
#include <set>
#include <stack>
#include <map>
#include <algorithm>
#include <queue>
#include "instruction.h"
#include "code.h"
#include "opcode.h"
#include "basicblock.h"
#include "phiinstruction.h"

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
    std::map<int, BasicBlock *> basicBlocks;

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
                case op_iload_0:
                case op_iload_1:
                case op_iload_2:
                case op_iload_3:
                case op_istore_0:
                case op_istore_1:
                case op_istore_2:
                case op_istore_3: {
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
        return basicBlocks[address];
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
                    basicBlocks[current->getStartingAddress()] = current;
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
                    basicBlocks[current->getStartingAddress()] = current;
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
            basicBlocks[current->getStartingAddress()] = current;
    }

    void linkBasicBlocks() {
        for (auto it = basicBlocks.begin(); it != basicBlocks.end(); ++it) {
            BasicBlock *block = it->second;
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

    void reversePostOrder(BasicBlock *block, std::stack<BasicBlock *> &reversePost, std::set<int> &visited) {
        visited.insert(block->getStartingAddress());
        for (auto b : block->getSuccessors()) {
            if (visited.count(b->getStartingAddress()) == 0)
                reversePostOrder(b, reversePost, visited);
        }

        reversePost.push(block);
    }

    bool containsAssignment(BasicBlock *block, int variableNumber) {
        for (auto &instruction : block->getInstructions()) {
            if (instruction.getOpCode() == op_istore_0) {
                if (variableNumber == 0)
                    return true;
            } else if (instruction.getOpCode() == op_istore_1) {
                if (variableNumber == 1)
                    return true;
            } else if (instruction.getOpCode() == op_istore_2) {
                if (variableNumber == 2)
                    return true;
            } else if (instruction.getOpCode() == op_istore_3) {
                if (variableNumber == 3)
                    return true;
            } else if (instruction.getOpCode() == op_istore) {
                int var = (int) instruction.getOperands()[0];
                if (var == variableNumber)
                    return true;
            }
        }

        return false;
    }

    std::vector<BasicBlock *> getAllBlocksWithAssignment(int variableNumber) {
        std::vector<BasicBlock *> blocks;
        for (auto it = basicBlocks.begin(); it != basicBlocks.end(); ++it) {
            if (containsAssignment(it->second, variableNumber))
                blocks.emplace_back(it->second);
        }
        return blocks;
    }

//    bool hasPhiIntruction(BasicBlock *block) {
//        for (auto &instruction : block->getInstructions()) {
//            if (instruction.getOpCode() == op_phi)
//                return true;
//        }
//        return false;
//    }

    void createSSA() {
        //Shows how to place the phi functions and rename variables:
        // http://www.cs.colostate.edu/~mstrout/CS553Fall06/slides/lecture17-SSA.pdf
        std::stack<BasicBlock *> reversePostOrderStack;
        std::vector<BasicBlock *> reversPostOrderList;
        std::set<int> visited;
        reversePostOrder(basicBlocks[0], reversePostOrderStack, visited);
        while (!reversePostOrderStack.empty()) {
            reversPostOrderList.emplace_back(reversePostOrderStack.top());
            reversePostOrderStack.pop();
        }

        std::map<int, std::set<BasicBlock *> *> dominatorTree;
        auto initial = new std::set<BasicBlock *>();
        initial->insert(basicBlocks[0]);
        dominatorTree[0] = initial;

        auto allBlocks = new std::set<BasicBlock *>();
        for (auto it = basicBlocks.begin(); it != basicBlocks.end(); ++it) {
            allBlocks->insert(it->second);
        }
        for (auto it = basicBlocks.begin(); it != basicBlocks.end(); ++it) {
            BasicBlock *b = it->second;
            if (b->getStartingAddress() != 0) {
                dominatorTree[b->getStartingAddress()] = new std::set<BasicBlock *>(*allBlocks);
            }
        }

        bool changed = true;
        while (changed) {
            changed = false;
            for (auto block : reversPostOrderList) {
                std::set<BasicBlock *> *newSet = nullptr;
                auto predecessors = block->getPredecessors();
                if (!predecessors.empty()) {
                    newSet = new std::set<BasicBlock *>(*dominatorTree[predecessors[0]->getStartingAddress()]);
                    for (int i = 1; i < predecessors.size(); i++) {
                        auto other = dominatorTree[predecessors[i]->getStartingAddress()];
                        auto intersection = new std::set<BasicBlock *>();
                        set_intersection(newSet->begin(), newSet->end(), other->begin(), other->end(),
                                         std::inserter(*intersection, intersection->begin()));
                        delete newSet;
                        newSet = intersection;
                    }
                }

                if (newSet == nullptr)
                    newSet = new std::set<BasicBlock *>();

                newSet->insert(block);
                if (*newSet != *dominatorTree[block->getStartingAddress()]) {
                    delete dominatorTree[block->getStartingAddress()];
                    dominatorTree[block->getStartingAddress()] = newSet;
                    changed = true;
                } else
                    delete newSet;
            }
        }

        std::map<int, std::set<BasicBlock *> *> dominanceFrontier;
        std::map<int, BasicBlock *> idom;

        //Compute immediate dominators
        for (auto it = basicBlocks.begin(); it != basicBlocks.end(); ++it) {
            BasicBlock *b = it->second;
            //Starting block has no immediate dominator
            if (b->getStartingAddress() != 0) {
                for (BasicBlock *dominator : *dominatorTree[b->getStartingAddress()]) {
                    if (b->getStartingAddress() != dominator->getStartingAddress()) {
                        bool isIdom = true;
                        for (BasicBlock *otherDominator : *dominatorTree[b->getStartingAddress()]) {
                            if (otherDominator->getStartingAddress() != b->getStartingAddress() &&
                                otherDominator->getStartingAddress() != dominator->getStartingAddress()) {
                                if (dominatorTree[otherDominator->getStartingAddress()]->count(dominator) > 0) {
                                    isIdom = false;
                                    break;
                                }
                            }
                        }

                        if (isIdom) {
                            idom[b->getStartingAddress()] = dominator;
                            break;
                        }
                    }
                }
            }
        }

        //Every node is in its own dominance frontier so initialize the frontiers with themselves
        for (auto it = basicBlocks.begin(); it != basicBlocks.end(); ++it) {
            BasicBlock *b = it->second;
            auto s = new std::set<BasicBlock *>();
            //s->insert(b);
            dominanceFrontier[b->getStartingAddress()] = s;
        }

        for (auto it = basicBlocks.begin(); it != basicBlocks.end(); ++it) {
            BasicBlock *b = it->second;
            if (b->getPredecessors().size() >= 2) {
                for (auto p : b->getPredecessors()) {
                    BasicBlock *runner = p;
                    while (runner->getStartingAddress() != idom[b->getStartingAddress()]->getStartingAddress()) {
                        dominanceFrontier[runner->getStartingAddress()]->insert(b);
                        runner = idom[runner->getStartingAddress()];
                    }
                }
            }
        }

        printf("Immediate dominators for method %s\n", this->name.c_str());
        for (auto &it : idom) {
            int blockAddress = it.first;
            printf("Block %d is the immediate dominator of Block %d\n", it.second->getStartingAddress(),
                   blockAddress);
        }


        printf("Dominance frontier for method %s\n", this->name.c_str());
        for (auto &it : dominanceFrontier) {
            int blockAddress = it.first;
            for (auto dominator : *it.second) {
                printf("Block %d is in the dominance frontier of Block %d\n", dominator->getStartingAddress(),
                       blockAddress);
            }
        }

        //Now we place the phi functions accordingly
        for (int variable = 0; variable < this->code->getMaxLocals(); variable++) {
            std::queue<int> worklist;
            std::set<int> everOnWorklist;
            std::set<int> alreadyHasPhiFunc;
            //Add all blocks containing an assignment to the worklist and to the ever on worklist
            for (auto block : getAllBlocksWithAssignment(variable)) {
                worklist.push(block->getStartingAddress());
                everOnWorklist.insert(block->getStartingAddress());
            }

            while (!worklist.empty()) {
                BasicBlock *n = basicBlocks[worklist.front()];
                worklist.pop();
                for (BasicBlock *d : *dominanceFrontier[n->getStartingAddress()]) {
                    if (alreadyHasPhiFunc.count(d->getStartingAddress()) == 0) {
                        d->getInstructions().(d->getInstructions().begin(), PhiInstruction(variable));
                        alreadyHasPhiFunc.insert(d->getStartingAddress());
                        if (everOnWorklist.count(d->getStartingAddress()) == 0) {
                            worklist.push(d->getStartingAddress());
                            everOnWorklist.insert(d->getStartingAddress());
                        }
                    }
                }
            }
        }

        //Now we go through and rename all the variables (actually we create new ones)
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

        //Translate the java bytecode instructions into our type of instruction
        createSSA();
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
