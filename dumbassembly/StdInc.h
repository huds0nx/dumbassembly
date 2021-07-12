#pragma once

#include <windows.h>
#include <assert.h>
#include <stdio.h>
#include <algorithm>
#include <hash_map>
#include <map>
#include <set>
#include <vector>

#include <StrongName.h>

#include "../shared/StdInc.h"
#include "../dotnetpe/StdInc.h"
#include "../vendor/FastDelegate/FastDelegate.h"
#include "../vendor/crypto++/base64.h"
#include "../vendor/crypto++/cryptlib.h"
#include "../vendor/crypto++/des.h"
#include "../vendor/crypto++/filters.h"
#include "../vendor/crypto++/modes.h"
#include "../vendor/DisasMSIL/DisasMSIL.h"
#include "../vendor/RebelDotNet/RebelDotNET.h"
#include "../vendor/zlib/zlib.h"

#include "Util.h"
#include "RebelFile.h"
#include "ResourceDecryptor.h"
#include "IProtectionStripper.h"
#include "AntiCodeSplicing.h"
#include "AntiImportHiding.h"
#include "AntiStringEncryption.h"
#include "AntiResourceEncryption.h"
#include "ReSigner.h"
#include "DumbAssembly.h"
