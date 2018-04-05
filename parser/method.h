#ifndef JAVACOMPILER_METHOD_H
#define JAVACOMPILER_METHOD_H

#include "qualifier.h"
#include "instruction.h"

class Method {
    private:
        Qualifier qualifiers[];
        Instruction instructions[];
};
#endif //JAVACOMPILER_METHOD_H
