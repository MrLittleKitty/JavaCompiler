#ifndef JAVACOMPILER_RENAMEHELPER_H
#define JAVACOMPILER_RENAMEHELPER_H

#include <vector>
#include <map>

class RenameHelper {
private:
    int counter;
    std::map<int, std::vector<int> *> variableStacks;

public:
    RenameHelper() : counter(1) {

    }

    int findVariableForNumber(int number) {
        for (auto entry : variableStacks) {
            for (int val : *entry.second) {
                if (val == number)
                    return entry.first;
            }
        }
        return -1;
    }

    void pop(int variable) {
        if (variableStacks.count(variable) != 0) {
            std::vector<int> *p = variableStacks[variable];
            p->pop_back();
            if (p->empty()) {
                delete p;
                variableStacks.erase(variable);
            }
        }
    }

    int top(int variable) {
        if (variableStacks.count(variable) == 0)
            genName(variable);

        return variableStacks[variable]->back();
    }

    void genName(int variable) {
        if (variableStacks.count(variable) == 0)
            variableStacks[variable] = new std::vector<int>();

        variableStacks[variable]->push_back(counter);
        counter++;
    }
};

#endif //JAVACOMPILER_RENAMEHELPER_H
