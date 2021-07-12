#include "StdInc.h"

namespace dotnetpe
{

    CustomModSigElem::CustomModSigElem ( StructureLayer* pStructureLayer )
        : SigElem ( pStructureLayer )
    {

    }

    CustomModSigElem::CustomModSigElem ( StructureLayer* pStructureLayer, const void* pData )
        : SigElem ( pStructureLayer, pData )
    {

    }

    CustomModSigElem::~CustomModSigElem ()
    {

    }

    void CustomModSigElem::Read ( SigElemStream* pStream )
    {
        byte ucElemType = pStream->ReadByte ();
        assert ( ucElemType == ELEMENT_TYPE_CMOD_REQD || ucElemType == ELEMENT_TYPE_CMOD_OPT );

        pStream->ReadTypeDefOrRefEncoded ();
    }

    bool CustomModSigElem::IsRequired () const
    {
        return m_pData[0] == ELEMENT_TYPE_CMOD_REQD;
    }

    Type* CustomModSigElem::GetType () const
    {
        SigElemStream stream ( m_pStructureLayer, m_pData + 1, 0x10, true );
        return stream.ReadTypeDefOrRefEncoded ().GetType ();
    }

}
