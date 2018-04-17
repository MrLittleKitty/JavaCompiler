#ifndef JAVACOMPILER_FIELD_H
#define JAVACOMPILER_FIELD_H

#include <vector>

class Field {
private:
    std::string name;
//    std::vector<std::string> qualifiers;
public:
    Field(std::string name)
    : name(name) {

    }
};

#endif //JAVACOMPILER_FIELD_H
