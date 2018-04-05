cmake_minimum_required(VERSION 3.9)
project(JavaCompiler)

set(CMAKE_CXX_STANDARD 11)
configure_file(testCases/Test1.class Test1.class COPYONLY)

add_executable(JavaCompiler main.cpp class.h method.h instruction.h operand.h qualifier.h attribute.h field.h parser.h)