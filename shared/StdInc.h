#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <assert.h>
#include <map>
#include <string>
#include <vector>

typedef BYTE byte;
typedef WORD word;
typedef DWORD dword;

typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

#define NUMELMS(arr) ( sizeof(arr)/sizeof(arr[0]) )

#include "Stream.h"
#include "MemoryStream.h"
#include "lazyvector.h"
#include "prevector.h"
#include "prebitset.h"
#include "prioritystack.h"
#include "Pool.h"
