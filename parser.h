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

struct Integer_info {
    unsigned int bytes;
};

struct Float_info {
    unsigned int bytes;
};

struct Long_info {
    unsigned int high_bytes;
    unsigned int low_bytes;
};

struct Double_info {
    unsigned int high_bytes;
    unsigned int low_bytes;
};

struct NameAndType_info {
    unsigned short name_index;
    unsigned short descriptor_index;
};

map<int, Class_info*> classMap;
map<int, Ref_info*> fieldRefMap;
map<int, Ref_info*> methodRefMap;
map<int, Ref_info*> interfaceMethodRefMap;
map<int, String_info*> stringInfoMap;
map<int, Integer_info*> integerMap;
map<int, Float_info*> floatMap;
map<int, Long_info*> longMap;
map<int, Double_info*> doubleMap;
map<int, NameAndType_info*> nameAndTypeMap;
map<int, char*> stringMap;


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
    result = result | (unsigned int) (lower1);

    return result;
}

static int getSizeFromTag(unsigned char tag) {
    if(tag == C_Class)
        return 2;
    else if(tag == C_Fieldref)
        return 4;
    else if(tag == C_Methodref)
        return 4;
    else if(tag == C_InterfaceMethodRef)
        return 4;
    else if(tag == C_String)
        return 2;
    else if(tag == C_Integer)
        return 4;
    else if(tag == C_Float)
        return 4;
    else if(tag == C_Long)
        return 8;
    else if(tag == C_Double)
        return 8;
    else if(tag == C_NameAndType)
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

static Class *parseClassFile(char const *fileName) {

    vector<char> byteCode = ReadAllBytes(fileName);
    int index = 0;

    if (!checkMagicNumber(byteCode, index))
        return nullptr;
    index += 4;

    if (!checkVersion(byteCode, index))
        return nullptr;
    index += 4;

    unsigned short constantPoolEntries = pack16BitInteger(byteCode[index], byteCode[index + 1]);
    index += 2;

    for (int poolIndex = 1; poolIndex < constantPoolEntries; poolIndex++) {
        unsigned char tag = (unsigned char)byteCode[index];

        if(tag == C_Utf8) {
            continue;
        }

        //TODO---Handle each constant type
        index += 1 + getSizeFromTag(tag);
    }
}
