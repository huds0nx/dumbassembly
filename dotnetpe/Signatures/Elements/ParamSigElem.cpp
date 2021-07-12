#include "StdInc.h"

namespace dotnetpe
{

    ParamSigElem::ParamSigElem ( StructureLayer* pStructureLayer )
        : SigElem ( pStructureLayer )
    {

    }

    ParamSigElem::ParamSigElem ( StructureLayer* pStructureLayer, const void* pData )
        : SigElem ( pStructureLayer, pData )
    {

    }

    ParamSigElem::~ParamSigElem ()
    {

    }

    void ParamSigElem::Read ( SigElemStream* pStream )
    {
        while ( pStream->CanReadCustomMod () )
            pStream->ReadCustomMod ();

        if ( pStream->PeekByte () == ELEMENT_TYPE_BYREF )
        {
            pStream->ReadByte ();
            assert ( pStream->CanReadType () );
            pStream->ReadType ();
        }
        else if ( pStream->PeekByte () == ELEMENT_TYPE_TYPEDBYREF )
        {
            pStream->ReadByte ();
        }
        else
        {
            assert ( pStream->CanReadType () );
            pStream->ReadType ();
        }
    }

}
