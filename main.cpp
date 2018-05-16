#include <iostream>
#include "class.h"
#include "parser.h"
#include "interpreter.h"

int main() {

    Class *c = parseClassFile("Test1.class");

    runProgram(c);

    return 0;
}