cmake_minimum_required(VERSION 3.9)
project(JavaCompiler)

set(CMAKE_CXX_STANDARD 11)

add_executable(JavaCompiler main.cpp parser/class.h parser/method.h parser/instruction.h parser/operand.h parser/qualifier.h parser/attribute.h parser/field.h parser/parser.cpp)