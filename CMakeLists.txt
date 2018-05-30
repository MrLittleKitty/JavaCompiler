cmake_minimum_required(VERSION 3.9)
project(JavaCompiler)

set(CMAKE_CXX_STANDARD 11)
configure_file(testCases/Test1.class Test1.class COPYONLY)
configure_file(testCases/Test2.class Test2.class COPYONLY)
configure_file(testCases/Test3.class Test3.class COPYONLY)
configure_file(testCases/Test4.class Test4.class COPYONLY)
configure_file(testCases/Test5.class Test5.class COPYONLY)

add_executable(JavaCompiler main.cpp class.h method.h instruction.h attribute.h field.h parser.h code.h interpreter.h interpreter.h opcode.h stackframe.h parserinfo.h basicblock.h ssainstruction.h phiinstruction.h)