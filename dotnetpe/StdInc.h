#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <WinTrust.h>
#include <StrongName.h>
#include "vendor/corhdr.h"

#include <algorithm>
#include <functional>
#include <map>
#include <string>
#include <vector>

#include "../vendor/DisasMSIL/DisasMSIL.h"
#include "../vendor/FastDelegate/FastDelegate.h"

#include "../shared/StdInc.h"
#include "../pe/StdInc.h"

#include "ILInstr.h"
#include "InstrBuffer.h"

#include "StreamLayer.h"
#include "TableLayer.h"
#include "StructureLayer.h"

#include "SigElem.h"
#include "Signature.h"
#include "Signatures/Elements/TypeDefOrRefEncodedSigElem.h"
#include "Signatures/Elements/CustomModSigElem.h"
#include "Signatures/Elements/ParamSigElem.h"
#include "Signatures/Elements/RetTypeSigElem.h"
#include "Signatures/Elements/ArrayShapeSigElem.h"
#include "Signatures/Elements/TypeSigElem.h"
#include "Signatures/FieldSig.h"
#include "SigElemStream.h"

#include "BasicBlock.h"
#include "BasicBlockPool.h"

#include "DotNetPE.h"
