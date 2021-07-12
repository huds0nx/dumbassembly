#include "StdInc.h"

namespace dotnetpe
{

    SigElemStream::SigElemStream ( StructureLayer* pStructureLayer, const void* pData, int iSize, bool bReadOnly )
        : MemoryStream ( pData, iSize, bReadOnly ), m_pStructureLayer ( pStructureLayer )
    {
        
    }

    SigElemStream::SigElemStream ( const SigElem* pElem, bool bSkipElemTypeByte )
        : MemoryStream ( pElem->m_pData + (bSkipElemTypeByte ? 1 : 0), 0x7FFFFFFF, true ),
          m_pStructureLayer ( pElem->m_pStructureLayer )
    {

    }

    SigElemStream::~SigElemStream ()
    {
        
    }

    bool SigElemStream::CanReadCustomMod () const
    {
        byte ucElemType = PeekByte ();
        return ucElemType == ELEMENT_TYPE_CMOD_REQD || ucElemType == ELEMENT_TYPE_CMOD_OPT;
    }

    bool SigElemStream::CanReadType () const
    {
        byte ucElemType = PeekByte ();
        switch ( ucElemType )
        {
            case ELEMENT_TYPE_BOOLEAN:
            case ELEMENT_TYPE_CHAR:
            case ELEMENT_TYPE_I1:
            case ELEMENT_TYPE_U1:
            case ELEMENT_TYPE_I2:
            case ELEMENT_TYPE_U2:
            case ELEMENT_TYPE_I4:
            case ELEMENT_TYPE_U4:
            case ELEMENT_TYPE_I8:
            case ELEMENT_TYPE_U8:
            case ELEMENT_TYPE_R4:
            case ELEMENT_TYPE_R8:
            case ELEMENT_TYPE_I:
            case ELEMENT_TYPE_U:
            case ELEMENT_TYPE_ARRAY:
            case ELEMENT_TYPE_CLASS:
            case ELEMENT_TYPE_FNPTR:
            case ELEMENT_TYPE_GENERICINST:
            case ELEMENT_TYPE_MVAR:
            case ELEMENT_TYPE_OBJECT:
            case ELEMENT_TYPE_PTR:
            case ELEMENT_TYPE_STRING:
            case ELEMENT_TYPE_SZARRAY:
            case ELEMENT_TYPE_VALUETYPE:
            case ELEMENT_TYPE_VAR:
                return true;
        }
        return false;
    }

    CustomModSigElem SigElemStream::ReadCustomMod ()
    {
        assert ( CanReadCustomMod () );
        return ReadElem < CustomModSigElem > ();
    }

    TypeDefOrRefEncodedSigElem SigElemStream::ReadTypeDefOrRefEncoded ()
    {
        return ReadElem < TypeDefOrRefEncodedSigElem > ();
    }

    ParamSigElem SigElemStream::ReadParam ()
    {
        return ReadElem < ParamSigElem > ();
    }

    RetTypeSigElem SigElemStream::ReadRetType ()
    {
        return ReadElem < RetTypeSigElem > ();
    }

    TypeSigElem SigElemStream::ReadType ()
    {
        return ReadElem < TypeSigElem > ();
    }

    ArrayShapeSigElem SigElemStream::ReadArrayShape ()
    {
        return ReadElem < ArrayShapeSigElem > ();
    }

}
