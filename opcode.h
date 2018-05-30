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

#endif //JAVACOMPILER_OPCODE_H
