#include "StdInc.h"

namespace dotnetpe
{

    int StringsIndexMetaFieldReader::Init ( TableLayer* pTableLayer, int iFieldOffset )
    {
        return StreamIndexMetaFieldReader::Init ( pTableLayer,
            pTableLayer->GetPE ()->GetStringsMetaStream (), 0x01, iFieldOffset );
    }

    int GUIDIndexMetaFieldReader::Init ( TableLayer* pTableLayer, int iFieldOffset )
    {
        return StreamIndexMetaFieldReader::Init ( pTableLayer,
            pTableLayer->GetPE ()->GetGUIDMetaStream (), 0x02, iFieldOffset );
    }

    int BlobIndexMetaFieldReader::Init ( TableLayer* pTableLayer, int iFieldOffset )
    {
        return StreamIndexMetaFieldReader::Init ( pTableLayer,
            pTableLayer->GetPE ()->GetBlobMetaStream (), 0x04, iFieldOffset );
    }

}
