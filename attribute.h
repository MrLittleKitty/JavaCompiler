#ifndef JAVACOMPILER_ATTRIBUTE_H
#define JAVACOMPILER_ATTRIBUTE_H

class Attribute {
private:
    std::string name;
    unsigned int attribute_length;
    unsigned char *info;
public:
    Attribute(std::string name, unsigned int attribute_length, unsigned char *info)
            : name(name), attribute_length(attribute_length), info(info) {

    }

    std::string getName() const {
        return name;
    }

    int getLength() const {
        return attribute_length;
    }

    unsigned char *getData() {
        return info;
    }
};

#endif //JAVACOMPILER_ATTRIBUTE_H
