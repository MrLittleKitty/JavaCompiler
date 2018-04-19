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
#define iload_0 0x1a
#define iload_1 0x1b
#define iload_2 0x1c
#define iload_3 0x1d

#define op_istore 54
#define istore_0 0x3b
#define istore_1 0x3c
#define istore_2 0x3d
#define istore_3 0x3e

#define op_iadd 96

#define op_if_icmpeq 159
#define op_if_icmpne 160
#define op_if_icmplt 161
#define op_if_icmpgt 163

#define op_ifeq 153
#define op_ifne 154
#define op_ifgt 157
#define op_iflt 155

#define op_invokespecial 0xb7
#define op_invokestatic 184
#define op_invokevirtual 0xb6
#define op_getstatic 0xb2

#define op_goto 167

#define op_return 177

#define op_lstore_3 42

#define op_bipush 0x10

#define op_iinc 132

#endif //JAVACOMPILER_OPCODE_H
