#include <iostream>
#include "class.h"
#include "parser.h"
#include "interpreter.h"

int main() {

//    Class *c1 = parseClassFile("Test1.class");
//    for (auto &method : c1->getMethods()) {
//        if (method.getName() == "main") {
//            std::cout << "Printing Basic Blocks for Main Method" << std::endl << std::endl;
//            method.printInstructions();
//        }
//    }
//    runProgram(c1);
//
//    printf("\n---\n");

//    Class *c2 = parseClassFile("Test2.class");
//    for (auto &method : c2->getMethods()) {
//        if (method.getName() == "main") {
//            std::cout << "Printing Basic Blocks for Main Method" << std::endl << std::endl;
//            method.printInstructions();
//        }
//    }
//    runProgram(c2);
//
//    printf("\n---\n");
//
//    Class *c3 = parseClassFile("Test3.class");
//    for (auto &method : c3->getMethods()) {
//        if (method.getName() == "main") {
//            std::cout << "Printing Basic Blocks for Main Method" << std::endl << std::endl;
//            method.printInstructions();
//        }
//    }
//    runProgram(c3);
//
//    printf("\n---\n");
//
    Class *c4 = parseClassFile("Test4.class");
//    for (auto &method : c4->getMethods()) {
//        if (method.getName() == "main") {
//            std::cout << "Printing Basic Blocks for Main Method" << std::endl << std::endl;
//            method.printInstructions();
//        }
//    }
//    runProgram(c4);
//
//    printf("\n---\n");
//
    Class *c5 = parseClassFile("Test5.class");
    for (auto &method : c5->getMethods()) {
        if (method.getName() == "main") {
            std::cout << "Printing Basic Blocks for Main Method" << std::endl << std::endl;
            method.printInstructions();
        }
    }
    runProgram(c5);

    printf("\n---\n");

    return 0;
}