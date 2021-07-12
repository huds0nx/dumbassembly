#include "StdInc.h"

namespace dotnetpe
{

    FieldSig::FieldSig ( StructureLayer* pStructureLayer, const void* pSig )
        : Signature ( pStructureLayer, pSig ), m_TypeSigElem ( pStructureLayer )
    {
        SigElemStream stream = GetStream ();
        assert ( stream.ReadByte () == IMAGE_CEE_CS_CALLCONV_FIELD );
        while ( stream.CanReadCustomMod () )
            stream.ReadCustomMod ();

        m_TypeSigElem = stream.ReadType ();
    }

}
