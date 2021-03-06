//   
//    Copyright (c) 2008 Daniel Pistelli.
//

//
// OpCodes
//

//
// One-byte opcodes
//

#pragma once

#define ILOPCODE_NOP					0x00
#define ILOPCODE_BREAK					0x01

#define ILOPCODE_LDARG_0				0x02
#define ILOPCODE_LDARG_1				0x03
#define ILOPCODE_LDARG_2				0x04
#define ILOPCODE_LDARG_3				0x05

#define ILOPCODE_LDLOC_0				0x06
#define ILOPCODE_LDLOC_1				0x07
#define ILOPCODE_LDLOC_2				0x08
#define ILOPCODE_LDLOC_3				0x09

#define ILOPCODE_STLOC_0				0x0A
#define ILOPCODE_STLOC_1				0x0B
#define ILOPCODE_STLOC_2				0x0C
#define ILOPCODE_STLOC_3				0x0D

#define ILOPCODE_LDARG_S				0x0E
#define ILOPCODE_LDARGA_S				0x0F
#define ILOPCODE_STARG_S				0x10

#define ILOPCODE_LDLOC_S				0x11
#define ILOPCODE_LDLOCA_S				0x12
#define ILOPCODE_STLOC_S				0x13

#define ILOPCODE_LDNULL					0x14
#define ILOPCODE_LDC_I4_M1				0x15
#define ILOPCODE_LDC_I4_0				0x16
#define ILOPCODE_LDC_I4_1				0x17
#define ILOPCODE_LDC_I4_2				0x18
#define ILOPCODE_LDC_I4_3				0x19
#define ILOPCODE_LDC_I4_4				0x1A
#define ILOPCODE_LDC_I4_5				0x1B
#define ILOPCODE_LDC_I4_6				0x1C
#define ILOPCODE_LDC_I4_7				0x1D
#define ILOPCODE_LDC_I4_8				0x1E
#define ILOPCODE_LDC_I4_S				0x1F
#define ILOPCODE_LDC_I4					0x20
#define ILOPCODE_LDC_I8					0x21
#define ILOPCODE_LDC_R4					0x22
#define ILOPCODE_LDC_R8					0x23

// #define ILOPCODE_?					0x24

#define ILOPCODE_DUP					0x25
#define ILOPCODE_POP					0x26
#define ILOPCODE_JMP					0x27

#define ILOPCODE_CALL					0x28
#define ILOPCODE_CALLI					0x29

#define ILOPCODE_RET					0x2A

#define ILOPCODE_BR_S					0x2B
#define ILOPCODE_BRFALSE_S				0x2C
#define ILOPCODE_BRTRUE_S				0x2D
#define ILOPCODE_BEQ_S					0x2E
#define ILOPCODE_BGE_S					0x2F
#define ILOPCODE_BGT_S					0x30
#define ILOPCODE_BLE_S					0x31
#define ILOPCODE_BLT_S					0x32
#define ILOPCODE_BNE_UN_S				0x33
#define ILOPCODE_BGE_UN_S				0x34
#define ILOPCODE_BGT_UN_S				0x35
#define ILOPCODE_BLE_UN_S				0x36
#define ILOPCODE_BLT_UN_S				0x37
#define ILOPCODE_BR						0x38
#define ILOPCODE_BRFALSE				0x39
#define ILOPCODE_BRTRUE					0x3A
#define ILOPCODE_BEQ					0x3B
#define ILOPCODE_BGE					0x3C
#define ILOPCODE_BGT					0x3D
#define ILOPCODE_BLE					0x3E
#define ILOPCODE_BLT					0x3F
#define ILOPCODE_BNE_UN					0x40
#define ILOPCODE_BGE_UN					0x41
#define ILOPCODE_BGT_UN					0x42
#define ILOPCODE_BLE_UN					0x43
#define ILOPCODE_BLT_UN					0x44

#define ILOPCODE_SWITCH					0x45

#define ILOPCODE_LDIND_I1				0x46
#define ILOPCODE_LDIND_U1				0x47
#define ILOPCODE_LDIND_I2				0x48
#define ILOPCODE_LDIND_U2				0x49
#define ILOPCODE_LDIND_I4				0x4A
#define ILOPCODE_LDIND_U4				0x4B
#define ILOPCODE_LDIND_I8				0x4C
#define ILOPCODE_LDIND_I				0x4D
#define ILOPCODE_LDIND_R4				0x4E
#define ILOPCODE_LDIND_R8				0x4F
#define ILOPCODE_LDIND_REF				0x50

#define ILOPCODE_STIND_REF				0x51
#define ILOPCODE_STIND_I1				0x52
#define ILOPCODE_STIND_I2				0x53
#define ILOPCODE_STIND_I4				0x54
#define ILOPCODE_STIND_I8				0x55
#define ILOPCODE_STIND_R4				0x56
#define ILOPCODE_STIND_R8				0x57

#define ILOPCODE_ADD					0x58
#define ILOPCODE_SUB					0x59
#define ILOPCODE_MUL					0x5A
#define ILOPCODE_DIV					0x5B
#define ILOPCODE_DIV_UN					0x5C
#define ILOPCODE_REM					0x5D
#define ILOPCODE_REM_UN					0x5E
#define ILOPCODE_AND					0x5F
#define ILOPCODE_OR						0x60
#define ILOPCODE_XOR					0x61
#define ILOPCODE_SHL					0x62
#define ILOPCODE_SHR					0x63
#define ILOPCODE_SHR_UN					0x64
#define ILOPCODE_NEG					0x65
#define ILOPCODE_NOT					0x66

#define ILOPCODE_CONV_I1				0x67
#define ILOPCODE_CONV_I2				0x68
#define ILOPCODE_CONV_I4				0x69
#define ILOPCODE_CONV_I8				0x6A
#define ILOPCODE_CONV_R4				0x6B
#define ILOPCODE_CONV_R8				0x6C
#define ILOPCODE_CONV_U4				0x6D
#define ILOPCODE_CONV_U8				0x6E

#define ILOPCODE_CALLVIRT				0x6F
#define ILOPCODE_CPOBJ					0x70
#define ILOPCODE_LDOBJ					0x71
#define ILOPCODE_LDSTR					0x72
#define ILOPCODE_NEWOBJ					0x73
#define ILOPCODE_CASTCLASS				0x74
#define ILOPCODE_ISINST					0x75
#define ILOPCODE_CONV_R_UN				0x76

//#define ILOPCODE_?					0x77
//#define ILOPCODE_?					0x78

#define ILOPCODE_UNBOX					0x79
#define ILOPCODE_THROW					0x7A

#define ILOPCODE_LDFLD					0x7B
#define ILOPCODE_LDFLDA					0x7C
#define ILOPCODE_STFLD					0x7D
#define ILOPCODE_LDSFLD					0x7E
#define ILOPCODE_LDSFLDA				0x7F
#define ILOPCODE_STSFLD					0x80

#define ILOPCODE_STOBJ					0x81

#define ILOPCODE_CONV_OVF_I1_UN			0x82
#define ILOPCODE_CONV_OVF_I2_UN			0x83
#define ILOPCODE_CONV_OVF_I4_UN			0x84
#define ILOPCODE_CONV_OVF_I8_UN			0x85
#define ILOPCODE_CONV_OVF_U1_UN			0x86
#define ILOPCODE_CONV_OVF_U2_UN			0x87
#define ILOPCODE_CONV_OVF_U4_UN			0x88
#define ILOPCODE_CONV_OVF_U8_UN			0x89
#define ILOPCODE_CONV_OVF_I_UN			0x8A
#define ILOPCODE_CONV_OVF_U_UN			0x8B

#define ILOPCODE_BOX					0x8C
#define ILOPCODE_NEWARR					0x8D
#define ILOPCODE_LDLEN					0x8E

#define ILOPCODE_LDELEMA				0x8F
#define ILOPCODE_LDELEM_I1				0x90
#define ILOPCODE_LDELEM_U1				0x91
#define ILOPCODE_LDELEM_I2				0x92
#define ILOPCODE_LDELEM_U2				0x93
#define ILOPCODE_LDELEM_I4				0x94
#define ILOPCODE_LDELEM_U4				0x95
#define ILOPCODE_LDELEM_I8				0x96
#define ILOPCODE_LDELEM_I				0x97
#define ILOPCODE_LDELEM_R4				0x98
#define ILOPCODE_LDELEM_R8				0x99
#define ILOPCODE_LDELEM_REF				0x9A

#define ILOPCODE_STELEM_I				0x9B
#define ILOPCODE_STELEM_I1				0x9C
#define ILOPCODE_STELEM_I2				0x9D
#define ILOPCODE_STELEM_I4				0x9E
#define ILOPCODE_STELEM_I8				0x9F
#define ILOPCODE_STELEM_R4				0xA0
#define ILOPCODE_STELEM_R8				0xA1
#define ILOPCODE_STELEM_REF				0xA2

#define ILOPCODE_LDELEM					0xA3	//
#define ILOPCODE_STELEM					0xA4	// Introduced with the framework 2.0
#define ILOPCODE_UNBOX_ANY				0xA5	//
//#define ILOPCODE_?					0xA6
//#define ILOPCODE_?					0xA7
//#define ILOPCODE_?					0xA8
//#define ILOPCODE_?					0xA9
//#define ILOPCODE_?					0xAA
//#define ILOPCODE_?					0xAB
//#define ILOPCODE_?					0xAC
//#define ILOPCODE_?					0xAD
//#define ILOPCODE_?					0xAE
//#define ILOPCODE_?					0xAF
//#define ILOPCODE_?					0xB0
//#define ILOPCODE_?					0xB1
//#define ILOPCODE_?					0xB2

#define ILOPCODE_CONV_OVF_I1			0xB3
#define ILOPCODE_CONV_OVF_U1			0xB4
#define ILOPCODE_CONV_OVF_I2			0xB5
#define ILOPCODE_CONV_OVF_U2			0xB6
#define ILOPCODE_CONV_OVF_I4			0xB7
#define ILOPCODE_CONV_OVF_U4			0xB8
#define ILOPCODE_CONV_OVF_I8			0xB9
#define ILOPCODE_CONV_OVF_U8			0xBA

//#define ILOPCODE_?					0xBB
//#define ILOPCODE_?					0xBC
//#define ILOPCODE_?					0xBD
//#define ILOPCODE_?					0xBE
//#define ILOPCODE_?					0xBF
//#define ILOPCODE_?					0xC0
//#define ILOPCODE_?					0xC1

#define ILOPCODE_REFANYVAL				0xC2
#define ILOPCODE_CKFINITE				0xC3

//#define ILOPCODE_?					0xC4
//#define ILOPCODE_?					0xC5

#define ILOPCODE_MKREFANY				0xC6

//#define ILOPCODE_?					0xC7
//#define ILOPCODE_?					0xC8
//#define ILOPCODE_?					0xC9
//#define ILOPCODE_?					0xCA
//#define ILOPCODE_?					0xCB
//#define ILOPCODE_?					0xCC
//#define ILOPCODE_?					0xCD
//#define ILOPCODE_?					0xCE
//#define ILOPCODE_?					0xCF

#define ILOPCODE_LDTOKEN				0xD0

#define ILOPCODE_CONV_U2				0xD1
#define ILOPCODE_CONV_U1				0xD2
#define ILOPCODE_CONV_I					0xD3
#define ILOPCODE_CONV_OVF_I				0xD4
#define ILOPCODE_CONV_OVF_U				0xD5

#define ILOPCODE_ADD_OVF				0xD6
#define ILOPCODE_ADD_OVF_UN				0xD7
#define ILOPCODE_MUL_OVF				0xD8
#define ILOPCODE_MUL_OVF_UN				0xD9
#define ILOPCODE_SUB_OVF				0xDA
#define ILOPCODE_SUB_OVF_UN				0xDB

#define ILOPCODE_ENDFINALLY				0xDC
#define ILOPCODE_LEAVE					0xDD
#define ILOPCODE_LEAVE_S				0xDE
#define ILOPCODE_STIND_I				0xDF
#define ILOPCODE_CONV_U					0xE0

//
// Two-bytes opcodes
//

#define ILOPCODE_ARGLIST				0x00FE
#define ILOPCODE_CEQ					0x01FE
#define ILOPCODE_CGT					0x02FE
#define ILOPCODE_CGT_UN					0x03FE
#define ILOPCODE_CLT					0x04FE
#define ILOPCODE_CLT_UN					0x05FE
#define ILOPCODE_LDFTN					0x06FE
#define ILOPCODE_LDVIRTFTN				0x07FE

//#define ILOPCODE_?					0x08FE

#define ILOPCODE_LDARG					0x09FE
#define ILOPCODE_LDARGA					0x0AFE
#define ILOPCODE_STARG					0x0BFE
#define ILOPCODE_LDLOC					0x0CFE
#define ILOPCODE_LDLOCA					0x0DFE
#define ILOPCODE_STLOC					0x0EFE
#define ILOPCODE_LOCALLOC				0x0FFE

//#define ILOPCODE_?					0x10FE

#define ILOPCODE_ENDFILTER				0x11FE
#define ILOPCODE_UNALIGNED_				0x12FE
#define ILOPCODE_VOLATILE_				0x13FE
#define ILOPCODE_TAIL_					0x14FE
#define ILOPCODE_INITOBJ				0x15FE

#define ILOPCODE_CONSTRAINED_			0x16FE	// Introduced with the framework 2.0

#define ILOPCODE_CPBLK					0x17FE
#define ILOPCODE_INITBLK				0x18FE

#define ILOPCODE_NO_					0x19FE	// Introduced with the framework 2.0

#define ILOPCODE_RETHROW				0x1AFE

//#define ILOPCODE_?					0x1BFE

#define ILOPCODE_SIZEOF					0x1CFE
#define ILOPCODE_REFANYTYPE_V2			0x1DFE
#define ILOPCODE_READONLY_				0x1EFE	// Introduced with the framework 2.0

#ifndef QWORD
typedef unsigned __int64 QWORD;
#endif

typedef unsigned int DISASMSIL_OFFSET;

#define MAX_DISASMMSIL_MNEMONIC		100

typedef struct _ILOPCODE_STRUCT
{
	DISASMSIL_OFFSET	Offset;
	unsigned int		Size;

	char				Mnemonic[MAX_DISASMMSIL_MNEMONIC];

} ILOPCODE_STRUCT, *PILOPCODE_STRUCT;

BOOL GetSingleMSILInstr(BYTE *pMemory, 
						UINT MemorySize, 
						DISASMSIL_OFFSET CodeBase,
						ILOPCODE_STRUCT *ilop);

BOOL DisasMSIL(BYTE *pMemory, 
			   UINT MemorySize,
			   DISASMSIL_OFFSET CodeBase,
			   ILOPCODE_STRUCT *iloparray,
			   UINT nOpcodeStructs,
			   UINT *nDisassembledInstr);