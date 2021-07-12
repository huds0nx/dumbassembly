#include "StdInc.h"

namespace dotnetpe
{

    ArrayShapeSigElem::ArrayShapeSigElem ( StructureLayer* pStructureLayer )
        : SigElem ( pStructureLayer )
    {

    }

    ArrayShapeSigElem::ArrayShapeSigElem ( StructureLayer* pStructureLayer, const void* pData )
        : SigElem ( pStructureLayer, pData )
    {

    }

    ArrayShapeSigElem::~ArrayShapeSigElem ()
    {

    }

    void ArrayShapeSigElem::Read ( SigElemStream* pStream )
    {
        uint uiRank = pStream->ReadVarUInt32 ();

        uint uiNumSizes = pStream->ReadVarUInt32 ();
        for ( uint i = 0; i < uiNumSizes; i++ )
        {
            uint uiSize = pStream->ReadVarUInt32 ();
        }

        uint uiNumLoBounds = pStream->ReadVarUInt32 ();
        for ( uint i = 0; i < uiNumLoBounds; i++ )
        {
            uint uiLoBound = pStream->ReadVarUInt32 ();
        }
    }

}
