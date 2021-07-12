#include "StdInc.h"

namespace dotnetpe
{

    RetTypeSigElem::RetTypeSigElem ( StructureLayer* pStructureLayer )
        : SigElem ( pStructureLayer )
    {

    }

    RetTypeSigElem::RetTypeSigElem ( StructureLayer* pStructureLayer, const void* pData )
        : SigElem ( pStructureLayer, pData )
    {

    }

    RetTypeSigElem::~RetTypeSigElem ()
    {

    }

    void RetTypeSigElem::Read ( SigElemStream* pStream )
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
        else if ( pStream->PeekByte () == ELEMENT_TYPE_VOID )
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
