#ifndef JAVACOMPILER_INTERPRETER_H
#define JAVACOMPILER_INTERPRETER_H

#include "class.h"
#include <stack>

struct StackFrame {
    int *localVariables;
    std::stack<int> operandStack;
};

static int constructOffset(unsigned char one, unsigned char two) {
    return (int) ((unsigned int) (one << 8) | two);
}

static void runInstructions(std::vector<Instruction> &instructions, StackFrame &currentFrame) {
    for (int i = 0; i < instructions.size(); i++) {
        Instruction instruction = instructions[i];
        switch (instruction.getOpCode()) {
            case op_return: {
                return;
            }
            case op_iconst_ml: {
                currentFrame.operandStack.push(-1);
                break;
            }
            case op_iconst_0: {
                currentFrame.operandStack.push(0);
                break;
            }
            case op_iconst_1: {
                currentFrame.operandStack.push(1);
                break;
            }
            case op_iconst_2: {
                currentFrame.operandStack.push(2);
                break;
            }
            case op_iconst_3: {
                currentFrame.operandStack.push(3);
                break;
            }
            case op_iconst_4: {
                currentFrame.operandStack.push(4);
                break;
            }
            case op_iconst_5: {
                currentFrame.operandStack.push(5);
                break;
            }
            case op_iadd: {
                int one = currentFrame.operandStack.top();
                currentFrame.operandStack.pop();
                int two = currentFrame.operandStack.top();
                currentFrame.operandStack.pop();
                currentFrame.operandStack.push(one + two);
                break;
            }
            case op_bipush: {
                int value = (int) instruction.getOperands()[0];
                currentFrame.operandStack.push(value);
                break;
            }

            case op_lstore_3:
            case iload_0:
            case iload_1:
            case iload_2:
            case iload_3:
            case istore_0:
            case istore_1:
            case istore_2:
            case istore_3: {
                break;
            }

            case op_iload:
            case op_istore:


            case op_if_icmpne:
            case op_if_icmpeq:
            case op_if_icmpgt:
            case op_if_icmplt:
            case op_ifeq:
            case op_ifne:
            case op_ifgt:
            case op_iflt:
            case op_goto:
            case op_invokestatic:
            case op_invokespecial:
            case op_getstatic:
            case op_invokevirtual:
            case op_iinc: {

            }
        }
    }
}

static void runProgram(Class *program) {

    for (auto method : program->getMethods()) {
        //Look for the main method by checking for the main descriptor
        if (method.getDescriptor() == "([Ljava/lang/String;)V") {
            StackFrame frame;
            runInstructions(method.getInstructions(), frame);
            break;
        }
    }
}

#endif //JAVACOMPILER_INTERPRETER_H
