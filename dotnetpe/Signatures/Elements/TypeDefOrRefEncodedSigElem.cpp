#include "StdInc.h"

namespace dotnetpe
{

    TypeDefOrRefEncodedSigElem::TypeDefOrRefEncodedSigElem ( StructureLayer* pStructureLayer )
        : SigElem ( pStructureLayer )
    {

    }

    TypeDefOrRefEncodedSigElem::TypeDefOrRefEncodedSigElem ( StructureLayer* pStructureLayer, const void* pData )
        : SigElem ( pStructureLayer, pData )
    {

    }

    TypeDefOrRefEncodedSigElem::~TypeDefOrRefEncodedSigElem ()
    {

    }

    void TypeDefOrRefEncodedSigElem::Read ( SigElemStream* pStream )
    {
        pStream->ReadVarUInt32 ();
    }

    Type* TypeDefOrRefEncodedSigElem::GetType () const
    {
        MemoryStream stream ( m_pData, 0x10, true );
        dword dwEncoded = stream.ReadVarUInt32 ();
        TypeDefOrRefEncoded* pEncoded = reinterpret_cast < TypeDefOrRefEncoded* > ( &dwEncoded );
        switch ( pEncoded->dwTag )
        {
            case 0:
                return m_pStructureLayer->GetModule ()->GetType ( pEncoded->dwIndex - 1 );

            case 1:
                return m_pStructureLayer->GetModule ()->GetReferencedType ( pEncoded->dwIndex - 1 );
        }
        return NULL;
    }

}
