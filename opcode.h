#ifndef JAVACOMPILER_OPCODE_H
#define JAVACOMPILER_OPCODE_H

#define op_iconst_ml 2
#define op_iconst_0 3
#define op_iconst_1 4
#define op_iconst_2 5
#define op_iconst_3 6
#define op_iconst_4 7
#define op_iconst_5 8

#define op_iload 21
#define op_iload_0 0x1a
#define op_iload_1 0x1b
#define op_iload_2 0x1c
#define op_iload_3 0x1d

#define op_istore 54
#define op_istore_0 0x3b
#define op_istore_1 0x3c
#define op_istore_2 0x3d
#define op_istore_3 0x3e

#define op_iadd 96

#define op_if_icmpeq 159
#define op_if_icmpne 160
#define op_if_icmplt 161
#define op_if_icmpge 162
#define op_if_icmpgt 163
#define op_if_icmple 164

#define op_ifeq 153
#define op_ifne 154
#define op_iflt 155
#define op_ifge 156
#define op_ifgt 157
#define op_ifle 0x9e


#define op_invokespecial 0xb7
#define op_invokestatic 184
#define op_invokevirtual 0xb6
#define op_getstatic 0xb2

#define op_goto 167

#define op_return 177

#define op_aload_0 42

#define op_bipush 0x10

#define op_iinc 132

#define op_phi (unsigned char)255

static std::string getStringFromOpCode(int opCode) {
    switch (opCode) {
        case op_iconst_ml:
            return "iconst_ml";
        case op_iconst_0:
            return "iconst_0";
        case op_iconst_1:
            return "iconst_1";
        case op_iconst_2:
            return "iconst_2";
        case op_iconst_3:
            return "iconst_3";
        case op_iconst_4:
            return "iconst_4";
        case op_iconst_5:
            return "iconst_5";

        case op_iload:
            return "iload";
        case op_iload_0:
            return "iload_0";
        case op_iload_1:
            return "iload_1";
        case op_iload_2:
            return "iload_2";
        case op_iload_3:
            return "iload_3";

        case op_istore:
            return "istore";
        case op_istore_0:
            return "istore_0";
        case op_istore_1:
            return "istore_1";
        case op_istore_2:
            return "istore_2";
        case op_istore_3:
            return "istore_3";

        case op_iadd:
            return "iadd";

        case op_if_icmpeq:
            return "if_icmpeq";
        case op_if_icmpne:
            return "if_icmpne";
        case op_if_icmplt:
            return "if_icmplt";
        case op_if_icmpge:
            return "if_icmpge";
        case op_if_icmpgt:
            return "if_icmpgt";
        case op_if_icmple:
            return "if_icmple";

        case op_ifeq:
            return "ifeq";
        case op_ifne:
            return "ifne";
        case op_iflt:
            return "iflt";
        case op_ifge:
            return "ifge";
        case op_ifgt:
            return "ifgt";
        case op_ifle:
            return "ifle";

        case op_invokespecial:
            return "invokespecial";
        case op_invokestatic:
            return "invokestatic";
        case op_invokevirtual:
            return "invokevirtual";
        case op_getstatic:
            return "getstatic";

        case op_goto:
            return "goto";
        case op_return:
            return "return";
        case op_aload_0:
            return "aload_0";
        case op_bipush:
            return "bipush";
        case op_iinc:
            return "iinc";
        case op_phi:
            return "phi";

        default:
            return "Unknown opcode";
    }
}

#endif //JAVACOMPILER_OPCODE_H
