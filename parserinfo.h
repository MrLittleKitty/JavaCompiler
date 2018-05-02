#ifndef JAVACOMPILER_PARSERINFO_H
#define JAVACOMPILER_PARSERINFO_H
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
#endif //JAVACOMPILER_PARSERINFO_H
