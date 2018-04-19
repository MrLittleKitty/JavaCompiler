#ifndef JAVACOMPILER_PARSER_H
#define JAVACOMPILER_PARSER_H

#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include "class.h"

#define C_Class 7
#define C_Fieldref 9
#define C_Methodref 10
#define C_InterfaceMethodRef 11
#define C_String 8
#define C_Integer 3
#define C_Float 4
#define C_Long 5
#define C_Double 6
#define C_NameAndType 12
#define C_Utf8 1

using namespace std;

struct Class_info {
    unsigned short name_index;
};

//Works for Fieldref, Methodref, and InterfaceMethodref
struct Ref_info {
    unsigned short class_index;
    unsigned short name_and_type_index;
};

struct String_info {
    unsigned short string_index;
};

struct Number_info {
    unsigned int bytes;
};

struct BigNumber_info {
    unsigned int high_bytes;
    unsigned int low_bytes;
};

struct NameAndType_info {
    unsigned short name_index;
    unsigned short descriptor_index;
};

static map<int, Class_info *> classMap;
static map<int, Ref_info *> fieldRefMap;
static map<int, Ref_info *> methodRefMap;
static map<int, Ref_info *> interfaceMethodRefMap;
static map<int, String_info *> stringInfoMap;
static map<int, Number_info *> integerMap;
static map<int, Number_info *> floatMap;
static map<int, BigNumber_info *> longMap;
static map<int, BigNumber_info *> doubleMap;
static map<int, NameAndType_info *> nameAndTypeMap;
static map<int, string> stringMap;


//struct Method_info {
//    unsigned short access_flags;
//    unsigned short name_index;
//    unsigned short descriptor_index;
//    unsigned short attributes_count;
//    Attribute_info **attributes;
//};
//
//vector<Method_info *> methodInfoList;
//vector<Attribute_info *> attributeInfoList;

static unsigned short pack16BitInteger(unsigned char upper, unsigned char lower) {
    unsigned short result = upper;
    result = result << 8;
    result = result | (unsigned short) lower;

    return result;
}

static unsigned int pack32BitInteger(unsigned char upper1, unsigned char upper2,
                                     unsigned char lower1, unsigned char lower2) {
    unsigned int result = upper1;
    result = result << 24;
    result = result | (unsigned int) (upper2 << 16);
    result = result | (unsigned int) (lower1 << 8);
    result = result | (unsigned int) (lower2);

    return result;
}

static int getSizeFromTag(unsigned char tag) {
    if (tag == C_Class)
        return 2;
    else if (tag == C_Fieldref)
        return 4;
    else if (tag == C_Methodref)
        return 4;
    else if (tag == C_InterfaceMethodRef)
        return 4;
    else if (tag == C_String)
        return 2;
    else if (tag == C_Integer)
        return 4;
    else if (tag == C_Float)
        return 4;
    else if (tag == C_Long)
        return 8;
    else if (tag == C_Double)
        return 8;
    else if (tag == C_NameAndType)
        return 4;
    else return 0;
}

static vector<char> ReadAllBytes(char const *filename) {
    ifstream ifs(filename, ios::binary | ios::ate);
    ifstream::pos_type pos = ifs.tellg();

    vector<char> result(pos);

    ifs.seekg(0, ios::beg);
    ifs.read(&result[0], pos);

    return result;
}

static bool checkMagicNumber(vector<char> &bytes, int index) {
    //The first 4 bytes of the bytecode spell the magic number CAFE BABE
    if ((unsigned char) bytes[index] != 0xCA)
        return false;

    if ((unsigned char) bytes[index + 1] != 0xFE)
        return false;

    if ((unsigned char) bytes[index + 2] != 0xBA)
        return false;

    if ((unsigned char) bytes[index + 3] != 0xBE)
        return false;

    return true;
}

static bool checkVersion(vector<char> &bytes, int index) {
    unsigned short majorVersion = pack16BitInteger(bytes[index + 2], bytes[index + 3]);
    return majorVersion >= 50; //50 is Java 6.0
}

static Attribute *parseAttributeInfo(vector<char> &bytes, int &index) {

    unsigned short attribute_name_index = pack16BitInteger(bytes[index], bytes[index + 1]);
    index += 2;

    unsigned int attribute_length = pack32BitInteger(bytes[index], bytes[index + 1], bytes[index + 2],
                                                     bytes[index + 3]);
    index += 4;

    unsigned char *info = new unsigned char[attribute_length];
    for (unsigned int i = 0; i < attribute_length; i++) {
        info[i] = (unsigned char) bytes[index];
        index += 1;
    }

    return new Attribute(stringMap[attribute_name_index], attribute_length, info);
}

static Code *parseCodeAttribute(vector<char> &bytes, int &index) {
    unsigned short attribute_name_index = pack16BitInteger(bytes[index], bytes[index + 1]);
    index += 2;

    unsigned int attribute_length = pack32BitInteger(bytes[index], bytes[index + 1], bytes[index + 2],
                                                     bytes[index + 3]);
    index += 4;

    unsigned short max_stack = pack16BitInteger(bytes[index], bytes[index + 1]);
    index += 2;

    unsigned short max_locals = pack16BitInteger(bytes[index], bytes[index + 1]);
    index += 2;

    unsigned int code_length = pack32BitInteger(bytes[index], bytes[index + 1], bytes[index + 2], bytes[index + 3]);
    index += 4;

    unsigned char *code = new unsigned char[code_length];
    for (unsigned int i = 0; i < code_length; i++) {
        code[i] = (unsigned char) bytes[index];
        index += 1;
    }

    unsigned short exeception_table_length = pack16BitInteger(bytes[index], bytes[index + 1]);
    index += 2;

    int exceptionByteSize = exeception_table_length * 8;
    unsigned char *exception_table = new unsigned char[exceptionByteSize];
    for (unsigned int i = 0; i < exceptionByteSize; i++) {
        exception_table[i] = (unsigned char) bytes[index];
        index += 1;
    }

    unsigned short attributes_count = pack16BitInteger(bytes[index], bytes[index + 1]);
    index += 2;

    Attribute **attributes = new Attribute *[attributes_count];

    for (int attributeIndex = 0; attributeIndex < attributes_count; attributeIndex++) {
        Attribute *attribute_info = parseAttributeInfo(bytes, index);
        attributes[attributeIndex] = attribute_info;
    }

    return new Code(max_stack, max_locals, code_length, code, exeception_table_length, exception_table,
                    attributes_count, attributes);
}

static std::vector<std::string> parseAccessFlag(unsigned short access_flag) {

    std::vector<std::string> qualifiers;

    if (access_flag & 0x0001 > 0)
        qualifiers.emplace_back("public");
    else if (access_flag & 0x0002 > 0)
        qualifiers.emplace_back("private");
    else if (access_flag & 0x0004 > 0)
        qualifiers.emplace_back("protected");

    if (access_flag & 0x0010 > 0)
        qualifiers.push_back("static");

    return qualifiers;
}

static Method *parseMethodInfo(vector<char> &bytes, int &index) {
    unsigned short access_flags = pack16BitInteger(bytes[index], bytes[index + 1]);
    index += 2;

    auto qualifiers = parseAccessFlag(access_flags);

    unsigned short name_index = pack16BitInteger(bytes[index], bytes[index + 1]);
    index += 2;

    unsigned short descriptor_index = pack16BitInteger(bytes[index], bytes[index + 1]);
    index += 2;

    unsigned short attributes_count = pack16BitInteger(bytes[index], bytes[index + 1]);
    index += 2;

    Attribute **attributes = new Attribute *[attributes_count - 1];

    int attributeIndex = 0;
    Code *code = nullptr;
    for (int i = 0; i < attributes_count; i++) {
        unsigned short attribute_name_index = pack16BitInteger(bytes[index], bytes[index + 1]);

        if (stringMap[attribute_name_index] == "Code")
            code = parseCodeAttribute(bytes, index);
        else {
            Attribute *attribute_info = parseAttributeInfo(bytes, index);
            attributes[attributeIndex] = attribute_info;
            attributeIndex++;
        }
    }

    return new Method(stringMap[name_index], stringMap[descriptor_index], code, qualifiers);
}

static void parseConstantPoolEntry(vector<char> &bytes, int index, unsigned char tag, int poolIndex) {
    if (tag == C_Class) {
        Class_info *info = new Class_info;
        info->name_index = pack16BitInteger(bytes[index], bytes[index + 1]);
        classMap[poolIndex] = info;
    } else if (tag == C_Fieldref || tag == C_Methodref || tag == C_InterfaceMethodRef) {
        Ref_info *info = new Ref_info;
        info->class_index = pack16BitInteger(bytes[index], bytes[index + 1]);
        info->name_and_type_index = pack16BitInteger(bytes[index + 2], bytes[index + 3]);

        if (tag == C_Fieldref)
            fieldRefMap[poolIndex] = info;
        else if (tag == C_Methodref)
            methodRefMap[poolIndex] = info;
        else
            interfaceMethodRefMap[poolIndex] = info;
    } else if (tag == C_String) {
        String_info *info = new String_info;
        info->string_index = pack16BitInteger(bytes[index], bytes[index + 1]);
        stringInfoMap[poolIndex] = info;
    } else if (tag == C_Integer || tag == C_Float) {
        Number_info *info = new Number_info;
        info->bytes = pack32BitInteger(bytes[index], bytes[index + 1], bytes[index + 2], bytes[index + 3]);

        if (tag == C_Integer)
            integerMap[poolIndex] = info;
        else
            floatMap[poolIndex] = info;
    } else if (tag == C_Long || tag == C_Double) {
        BigNumber_info *info = new BigNumber_info;
        info->high_bytes = pack32BitInteger(bytes[index], bytes[index + 1], bytes[index + 2], bytes[index + 3]);
        info->low_bytes = pack32BitInteger(bytes[index + 4], bytes[index + 5], bytes[index + 6], bytes[index + 7]);

        if (tag == C_Long)
            longMap[poolIndex] = info;
        else
            doubleMap[poolIndex] = info;
    } else if (tag == C_NameAndType) {
        NameAndType_info *info = new NameAndType_info;
        info->name_index = pack16BitInteger(bytes[index], bytes[index + 1]);
        info->descriptor_index = pack16BitInteger(bytes[index + 2], bytes[index + 3]);
        nameAndTypeMap[poolIndex] = info;
    }
}

static Class *parseClassFile(char const *fileName) {

    vector<char> classFileBytes = ReadAllBytes(fileName);
    int index = 0;

    if (!checkMagicNumber(classFileBytes, index))
        return nullptr;
    index += 4;

    if (!checkVersion(classFileBytes, index))
        return nullptr;
    index += 4;

    //"The value of the constant_pool_count item is equal to the number of entries in the constant_pool table plus one"
    unsigned short constantPoolEntries = pack16BitInteger(classFileBytes[index], classFileBytes[index + 1]);
    index += 2;

    for (int poolIndex = 1; poolIndex < constantPoolEntries; poolIndex++) {
        unsigned char tag = (unsigned char) classFileBytes[index];
        index += 1;

        if (tag == C_Utf8) {
            unsigned short length = pack16BitInteger(classFileBytes[index], classFileBytes[index + 1]);
            index += 2;

            vector<char> charsVec;

            for (int byteIndex = 0; byteIndex < length; byteIndex++) {
                unsigned char x = (unsigned char) classFileBytes[index + byteIndex];
                if ((unsigned char) (x & 0x80) == 0x00) {
                    charsVec.push_back(x);
                    continue;
                }

                byteIndex += 1;
                unsigned char y = (unsigned char) classFileBytes[index + byteIndex];
                if ((unsigned char) (x & 0xE0) == 0xC0
                    && (unsigned char) (y & 0xC0) == 0x80) {
                    charsVec.push_back((unsigned char) (((x & 0x1f) << 6) + (y & 0x3f)));
                    continue;
                }

                byteIndex += 1;
                unsigned char z = (unsigned char) classFileBytes[index + byteIndex];
                if ((unsigned char) (x & 0xF0) == 0xE0
                    && (unsigned char) (y & 0xC0) == 0x80
                    && (unsigned char) (z & 0xC0) == 0x80) {
                    charsVec.push_back((unsigned char) (((x & 0xf) << 12) + ((y & 0x3f) << 6) + (z & 0x3f)));
                }
            }

            std::string str(charsVec.begin(), charsVec.end());
            stringMap[poolIndex] = str;

            index += length;
            continue;
        }

        parseConstantPoolEntry(classFileBytes, index, tag, poolIndex);

        index += getSizeFromTag(tag);
    }

    unsigned short access_flags = pack16BitInteger(classFileBytes[index], classFileBytes[index + 1]);
    index += 2;

    unsigned short this_class = pack16BitInteger(classFileBytes[index], classFileBytes[index + 1]);
    index += 2;

    unsigned short super_class = pack16BitInteger(classFileBytes[index], classFileBytes[index + 1]);
    index += 2;

    unsigned short interface_count = pack16BitInteger(classFileBytes[index], classFileBytes[index + 1]);
    index += 2;

    for (int interfacesIndex = 0; interfacesIndex < interface_count; interfacesIndex++) {
        unsigned short interfaceConstantPoolIndex = pack16BitInteger(classFileBytes[index], classFileBytes[index + 1]);
        index += 2;
    }

    unsigned short fields_count = pack16BitInteger(classFileBytes[index], classFileBytes[index + 1]);
    index += 2;

    for (int fieldsIndex = 0; fieldsIndex < fields_count; fieldsIndex++) {
    }

    unsigned short methods_count = pack16BitInteger(classFileBytes[index], classFileBytes[index + 1]);
    index += 2;

    std::vector<Method> methods;
    for (int methodsIndex = 0; methodsIndex < methods_count; methodsIndex++) {
        Method *method = parseMethodInfo(classFileBytes, index);
        methods.push_back(*method);
        delete method;
    }

    unsigned short attributes_count = pack16BitInteger(classFileBytes[index], classFileBytes[index + 1]);
    index += 2;

    std::vector<Attribute> attributes;
    for (int attributesIndex = 0; attributesIndex < attributes_count; attributesIndex++) {
        Attribute *attribute = parseAttributeInfo(classFileBytes, index);
        attributes.push_back(*attribute);
        delete attribute;
    }

    //Now we begin linking together all the parsed data into a "nice" c++ class structure
    Class *instance = new Class(stringMap[classMap[this_class]->name_index], methods, attributes);

    return instance;
}

#endif //JAVACOMPILER_PARSER_H