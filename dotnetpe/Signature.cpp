#include "StdInc.h"

namespace dotnetpe
{

    Signature::Signature ( StructureLayer* pStructureLayer, const void* pSig )
        : m_pStructureLayer ( pStructureLayer ), m_pSig ( (const byte *)pSig )
    {
        MemoryStream stream ( m_pSig, 0x7FFFFFFF, true );
        m_dwLength = stream.ReadVarUInt32 ();
        m_pSig += stream.GetPosition ();
    }

    SigElemStream Signature::GetStream ()
    {
        return SigElemStream ( m_pStructureLayer, m_pSig, m_dwLength, true );
    }

}
