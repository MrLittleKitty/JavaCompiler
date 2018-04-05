#include <iostream>
#include <vector>
#include <fstream>
#include "class.h"

using namespace std;

static vector<char> ReadAllBytes(char const* filename) {
    ifstream ifs(filename, ios::binary|ios::ate);
    ifstream::pos_type pos = ifs.tellg();

    vector<char>  result(pos);

    ifs.seekg(0, ios::beg);
    ifs.read(&result[0], pos);

    return result;
}

static Class parseClassFile(char const* fileName) {

    vector<char> byteCode = ReadAllBytes(fileName);

    for(int i = 0; i < byteCode.size(); i++)
}