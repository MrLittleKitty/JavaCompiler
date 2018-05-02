#ifndef JAVACOMPILER_STACKFRAME_H
#define JAVACOMPILER_STACKFRAME_H

#include <stack>

class StackFrame {
private:
    int *localVariables;
    std::stack<int> *operandStack;
    StackFrame *parentFrame;
public:
    StackFrame(unsigned short localVariableSize, StackFrame *parentFrame) :
            parentFrame(parentFrame) {

        localVariables = new int[localVariableSize];
        if (parentFrame == nullptr)
            operandStack = new std::stack<int>();
        else
            operandStack = parentFrame->operandStack;
    }

    ~StackFrame() {
        delete[] localVariables;
        if (parentFrame == nullptr)
            delete operandStack;
    }

    void pushOperand(int value) {
        operandStack->push(value);
    }

    int popOperand() {
        int value = operandStack->top();
        operandStack->pop();
        return value;
    }

    int getLocalVariable(int index) {
        return localVariables[index];
    }

    void setLocalVariable(int index, int value) {
        localVariables[index] = value;
    }

    StackFrame *getParentFrame() const {
        return parentFrame;
    }
};

#endif //JAVACOMPILER_STACKFRAME_H
