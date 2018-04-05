#include <iostream>
#include <vector>
#include <fstream>
#include "class.h"

using namespace std;

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


    }
}
