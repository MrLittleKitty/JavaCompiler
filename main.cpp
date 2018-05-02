#include <iostream>
#include "class.h"
#include "parser.h"
#include "interpreter.h"

int main() {

    Class *c = parseClassFile("Test3.class");

    runProgram(c);

    return 0;
}