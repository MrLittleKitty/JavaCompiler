#ifndef JAVACOMPILER_INTERPRETER_H
#define JAVACOMPILER_INTERPRETER_H

#include "class.h"
#include "stackframe.h"
#include <stack>


static int constructOffset(unsigned char one, unsigned char two) {
    return (int) ((unsigned int) (one << 8) | two);
}

static int getIndexOfByteCode(std::vector<Instruction> &instructions, int byteCodeIndex) {
    for (int i = 0; i < instructions.size(); i++) {
        if (instructions[i].getByteCodeIndex() == byteCodeIndex)
            return i;
    }
    return -1;
}

static void runInstructions(Class *program, std::vector<Instruction> &instructions, StackFrame &currentFrame) {
    for (int i = 0; i < instructions.size(); i++) {
        Instruction instruction = instructions[i];
        switch (instruction.getOpCode()) {
            case op_return: {
                return;
            }
            case op_iconst_ml: {
                currentFrame.pushOperand(-1);
                break;
            }
            case op_iconst_0: {
                currentFrame.pushOperand(0);
                break;
            }
            case op_iconst_1: {
                currentFrame.pushOperand(1);
                break;
            }
            case op_iconst_2: {
                currentFrame.pushOperand(2);
                break;
            }
            case op_iconst_3: {
                currentFrame.pushOperand(3);
                break;
            }
            case op_iconst_4: {
                currentFrame.pushOperand(4);
                break;
            }
            case op_iconst_5: {
                currentFrame.pushOperand(5);
                break;
            }
            case op_iadd: {
                int one = currentFrame.popOperand();
                int two = currentFrame.popOperand();
                currentFrame.pushOperand(one + two);
                break;
            }
            case op_bipush: {
                int value = (int) instruction.getOperands()[0];
                currentFrame.pushOperand(value);
                break;
            }

            case op_aload_0: {
                //Loads an object reference but we aren't handling objects
                break;
            }

            case iload_0: {
                currentFrame.pushOperand(currentFrame.getLocalVariable(0));
                break;
            }
            case iload_1: {
                currentFrame.pushOperand(currentFrame.getLocalVariable(1));
                break;
            }
            case iload_2: {
                currentFrame.pushOperand(currentFrame.getLocalVariable(2));
                break;
            }
            case iload_3: {
                currentFrame.pushOperand(currentFrame.getLocalVariable(3));
                break;
            }

            case istore_0: {
                int value = currentFrame.popOperand();
                currentFrame.setLocalVariable(0, value);
                break;
            }
            case istore_1: {
                int value = currentFrame.popOperand();
                currentFrame.setLocalVariable(1, value);
                break;
            }
            case istore_2: {
                int value = currentFrame.popOperand();
                currentFrame.setLocalVariable(2, value);
                break;
            }
            case istore_3: {
                int value = currentFrame.popOperand();
                currentFrame.setLocalVariable(3, value);
                break;
            }

            case op_iload: {
                unsigned char index = instruction.getOperands()[0];
                currentFrame.pushOperand(currentFrame.getLocalVariable(index));
                break;
            }
            case op_istore: {
                unsigned char index = instruction.getOperands()[0];
                int value = currentFrame.popOperand();
                currentFrame.setLocalVariable(index, value);
                break;
            }


            case op_if_icmpne: {
                int value1 = currentFrame.popOperand();
                int value2 = currentFrame.popOperand();
                if (value1 != value2) {
                    int offset = constructOffset(instruction.getOperands()[0], instruction.getOperands()[1]);
                    //The loop is going to increment it by one after this runs
                    i = getIndexOfByteCode(instructions, instruction.getByteCodeIndex() + offset) - 1;
                }
                break;
            }
            case op_if_icmpeq: {
                int value1 = currentFrame.popOperand();
                int value2 = currentFrame.popOperand();
                if (value1 == value2) {
                    int offset = constructOffset(instruction.getOperands()[0], instruction.getOperands()[1]);
                    //The loop is going to increment it by one after this runs
                    i = getIndexOfByteCode(instructions, instruction.getByteCodeIndex() + offset) - 1;
                }
                break;
            }
            case op_if_icmpgt: {
                int value1 = currentFrame.popOperand();
                int value2 = currentFrame.popOperand();
                if (value1 > value2) {
                    int offset = constructOffset(instruction.getOperands()[0], instruction.getOperands()[1]);
                    //The loop is going to increment it by one after this runs
                    i = getIndexOfByteCode(instructions, instruction.getByteCodeIndex() + offset) - 1;
                }
            }
            case op_if_icmplt: {
                int value1 = currentFrame.popOperand();
                int value2 = currentFrame.popOperand();
                if (value1 < value2) {
                    int offset = constructOffset(instruction.getOperands()[0], instruction.getOperands()[1]);
                    //The loop is going to increment it by one after this runs
                    i = getIndexOfByteCode(instructions, instruction.getByteCodeIndex() + offset) - 1;
                }
                break;
            }

            case op_if_icmple: {
                int value1 = currentFrame.popOperand();
                int value2 = currentFrame.popOperand();
                if (value1 <= value2) {
                    int offset = constructOffset(instruction.getOperands()[0], instruction.getOperands()[1]);
                    //The loop is going to increment it by one after this runs
                    i = getIndexOfByteCode(instructions, instruction.getByteCodeIndex() + offset) - 1;
                }
                break;
            }

            case op_if_icmpge: {
                int value1 = currentFrame.popOperand();
                int value2 = currentFrame.popOperand();
                if (value1 >= value2) {
                    int offset = constructOffset(instruction.getOperands()[0], instruction.getOperands()[1]);
                    //The loop is going to increment it by one after this runs
                    i = getIndexOfByteCode(instructions, instruction.getByteCodeIndex() + offset) - 1;
                }
                break;
            }

            case op_ifeq: {
                int value = currentFrame.popOperand();
                if (value == 0) {
                    int offset = constructOffset(instruction.getOperands()[0], instruction.getOperands()[1]);
                    //The loop is going to increment it by one after this runs
                    i = getIndexOfByteCode(instructions, instruction.getByteCodeIndex() + offset) - 1;
                }
                break;
            }
            case op_ifne: {
                int value = currentFrame.popOperand();
                if (value != 0) {
                    int offset = constructOffset(instruction.getOperands()[0], instruction.getOperands()[1]);
                    //The loop is going to increment it by one after this runs
                    i = getIndexOfByteCode(instructions, instruction.getByteCodeIndex() + offset) - 1;
                }
                break;
            }
            case op_ifgt: {
                int value = currentFrame.popOperand();
                if (value > 0) {
                    int offset = constructOffset(instruction.getOperands()[0], instruction.getOperands()[1]);
                    //The loop is going to increment it by one after this runs
                    i = getIndexOfByteCode(instructions, instruction.getByteCodeIndex() + offset) - 1;
                }
                break;
            }
            case op_iflt: {
                int value = currentFrame.popOperand();
                if (value < 0) {
                    int offset = constructOffset(instruction.getOperands()[0], instruction.getOperands()[1]);
                    //The loop is going to increment it by one after this runs
                    i = getIndexOfByteCode(instructions, instruction.getByteCodeIndex() + offset) - 1;
                }
                break;
            }

            case op_ifge: {
                int value = currentFrame.popOperand();
                if (value >= 0) {
                    int offset = constructOffset(instruction.getOperands()[0], instruction.getOperands()[1]);
                    //The loop is going to increment it by one after this runs
                    i = getIndexOfByteCode(instructions, instruction.getByteCodeIndex() + offset) - 1;
                }
                break;
            }

            case op_ifle: {
                int value = currentFrame.popOperand();
                if (value <= 0) {
                    int offset = constructOffset(instruction.getOperands()[0], instruction.getOperands()[1]);
                    //The loop is going to increment it by one after this runs
                    i = getIndexOfByteCode(instructions, instruction.getByteCodeIndex() + offset) - 1;
                }
                break;
            }
            case op_goto: {
                int offset = constructOffset(instruction.getOperands()[0], instruction.getOperands()[1]);
                //The loop is going to increment it by one after this runs
                i = getIndexOfByteCode(instructions, instruction.getByteCodeIndex() + offset) - 1;
                break;
            }

            case op_iinc: {
                unsigned char index = instruction.getOperands()[0];
                char constant = instruction.getOperands()[1];
                int newConstant = (int) constant; //Sign extend the signed constant into an int
                currentFrame.setLocalVariable(index, currentFrame.getLocalVariable(index) + newConstant);
                break;
            }

            case op_invokestatic: {
                int index = constructOffset(instruction.getOperands()[0], instruction.getOperands()[1]);
                std::string methodName = program->getMethodNameFromConstantPool(index);
                if (methodName == "printInt") {
                    int value = currentFrame.popOperand();
                    printf("%d", value);
                } else {
                    for (auto &method : program->getMethods()) {
                        if (method.getName() == methodName) {
                            StackFrame *frame = new StackFrame(method.getCode()->getMaxLocals(), &currentFrame);
                            runInstructions(program, method.getInstructions(), *frame);
                            delete frame;
                            break;
                        }
                    }
                }
                break;
            }

            case op_getstatic: {
                //Loads a static value but we aren't handling that
                break;
            }
            case op_invokespecial: {
                //Invokes a virtual method but we aren't handling that
                break;
            }
            case op_invokevirtual: {
                //Invokes a virtual method but we aren't handling that
                break;
            }

        }
    }
}

static void runProgram(Class *program) {

    for (auto &method : program->getMethods()) {
        //Look for the main method by checking for the main descriptor
        if (method.getName() == "main" && method.getDescriptor() == "([Ljava/lang/String;)V") {
            StackFrame *frame = new StackFrame(method.getCode()->getMaxLocals(), nullptr);
            runInstructions(program, method.getInstructions(), *frame);
            delete frame;
            break;
        }
    }
}

#endif //JAVACOMPILER_INTERPRETER_H
