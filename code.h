#ifndef JAVACOMPILER_CODE_H
#define JAVACOMPILER_CODE_H

#include "attribute.h"

class Code {
private:
    unsigned short max_stack;
    unsigned short max_locals;

    unsigned int code_length;
    unsigned char *code;

    unsigned short exeception_table_length;
    unsigned char *exception_table;

    unsigned short attributes_count;
    Attribute **attributes;
public:
    Code(unsigned short stack, unsigned short locals, unsigned int codeLength, unsigned char *code,
         unsigned short exceptionsLength, unsigned char *exceptions,
         unsigned short attributesLength, Attribute **attributes)
            : max_stack(stack), max_locals(locals), code_length(codeLength), code(code),
              exeception_table_length(exceptionsLength), exception_table(exceptions),
              attributes_count(attributesLength), attributes(attributes) {

    }

    unsigned short getMaxStack() const {
        return max_stack;
    }

    unsigned short getMaxLocals() const {
        return max_locals;
    }

    unsigned int getSize() const {
        return code_length;
    }

    unsigned char *getByteCode() {
        return code;
    }

    unsigned short getAttributesSize() const {
        return attributes_count;
    }

    Attribute **getAttributes() {
        return attributes;
    }
};

#endif //JAVACOMPILER_CODE_H
