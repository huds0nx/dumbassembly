#pragma once

#ifndef QWORD
typedef unsigned __int64 QWORD;
#endif

#define REBEL_NET_SIGNATURE 0x54454E4C45424552  // REBELNET

#pragma pack(1)
typedef struct _REBEL_NET_BASE
{
	QWORD Signature;

	DWORD Reserved;			

	QWORD Mask;

	DWORD Flags;			// .NET Header Flags
	DWORD EntryPointToken;  // .NET Header Entrypoint

	DWORD StreamsOffset;
	WORD NumberOfStreams;

	DWORD MethodsOffset;
	DWORD NumberOfMethods;

} REBEL_NET_BASE, *PREBEL_NET_BASE;
#pragma pack()

#define REBEL_BASE_MASK_FLAGS				1
#define REBEL_BASE_MASK_ENTRYPOINT			2

#define REBEL_BASE_MASK_DELETE_METHODS		0x8000000000000000

#pragma pack(1)
typedef struct _REBEL_METHOD
{
	DWORD Token;

	DWORD Mask;

	DWORD RVA;						// information only

	WORD Flags;
	WORD ImplFlags;

	WORD MaxStack;
	WORD HeaderFlags;				

	DWORD ParamList;				

	DWORD NameOffsetOrSize;			// offset into the #Strings stream
	DWORD SignatureOffsetOrSize;	// offset into the #Blob stream
	DWORD LocalVarSigOffsetOrSize;  // offset into the #Blob stream

	DWORD CodeSize;

	DWORD ExtraSectionsSize;

} REBEL_METHOD, *PREBEL_METHOD;
#pragma pack()

#define REBEL_METHOD_MASK_FLAGS				1
#define REBEL_METHOD_MASK_IMPLFLAGS			2
#define REBEL_METHOD_MASK_MAXSTACK			4
#define REBEL_METHOD_MASK_HEADERFLAGS		8
#define REBEL_METHOD_MASK_PARAMLIST			16
#define REBEL_METHOD_MASK_NAME				32
#define REBEL_METHOD_MASK_SIGNATURE			64
#define REBEL_METHOD_MASK_LOCVARSIG			128

#define REBEL_METHOD_MASK_ALL				255

// reference in the stream instead of size
#define REBEL_METHOD_MASK_LOCVARSIGOFFSET	0x20000000
#define REBEL_METHOD_MASK_SIGOFFSET			0x40000000
#define REBEL_METHOD_MASK_NAMEOFFSET		0x80000000