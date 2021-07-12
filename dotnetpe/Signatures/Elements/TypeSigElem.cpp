#include "StdInc.h"

namespace dotnetpe
{

    TypeSigElem::TypeSigElem ( StructureLayer* pStructureLayer )
        : SigElem ( pStructureLayer )
    {

    }

    TypeSigElem::TypeSigElem ( StructureLayer* pStructureLayer, const void* pData )
        : SigElem ( pStructureLayer, pData )
    {

    }

    TypeSigElem::~TypeSigElem ()
    {

    }

    void TypeSigElem::Read ( SigElemStream* pStream )
    {
        switch ( pStream->ReadByte () )
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
            case ELEMENT_TYPE_STRING:
            {
                return;
            }

            case ELEMENT_TYPE_ARRAY:
            {
                pStream->ReadType ();
                pStream->ReadArrayShape ();
                break;
            }

            case ELEMENT_TYPE_CLASS:
            {
                pStream->ReadTypeDefOrRefEncoded ();
                break;
            }

            case ELEMENT_TYPE_FNPTR:
            {
                break;
            }

            case ELEMENT_TYPE_GENERICINST:
            {
                byte ucKind = pStream->ReadByte ();
                assert ( ucKind == ELEMENT_TYPE_CLASS || ucKind == ELEMENT_TYPE_VALUETYPE );
                pStream->ReadTypeDefOrRefEncoded ();
                uint uiGenArgCount = pStream->ReadVarUInt32 ();
                for ( uint i = 0; i < uiGenArgCount; i++ )
                    pStream->ReadType ();
                break;
            }

            case ELEMENT_TYPE_MVAR:
            {
                pStream->ReadVarUInt32 ();
                break;
            }

            case ELEMENT_TYPE_OBJECT:
            {
                break;
            }

            case ELEMENT_TYPE_PTR:
            {
                while ( pStream->CanReadCustomMod () )
                    pStream->ReadCustomMod ();

                if ( pStream->PeekByte () == ELEMENT_TYPE_VOID )
                    pStream->ReadByte ();
                else
                    pStream->ReadType ();
                break;
            }

            case ELEMENT_TYPE_SZARRAY:
            {
                while ( pStream->CanReadCustomMod () )
                    pStream->ReadCustomMod ();

                pStream->ReadType ();
                break;
            }

            case ELEMENT_TYPE_VALUETYPE:
            {
                pStream->ReadTypeDefOrRefEncoded ();
                break;
            }

            case ELEMENT_TYPE_VAR:
            {
                pStream->ReadVarUInt32 ();
                break;
            }

            default:
            {
                assert ( false );
            }
        }
    }

    CorElementType TypeSigElem::GetElemType () const
    {
        return (CorElementType)*m_pData;
    }

    TypeSigElem TypeSigElem::GetArrayType () const
    {
        assert ( GetElemType () == ELEMENT_TYPE_ARRAY );
        SigElemStream stream ( this, true );
        return stream.ReadType ();
    }

    ArrayShapeSigElem TypeSigElem::GetArrayShape () const
    {
        assert ( GetElemType () == ELEMENT_TYPE_ARRAY );
        SigElemStream stream ( this, true );
        stream.ReadType ();
        return stream.ReadArrayShape ();
    }

    Type* TypeSigElem::GetClass () const
    {
        assert ( GetElemType () == ELEMENT_TYPE_CLASS );
        SigElemStream stream ( this, true );
        return stream.ReadTypeDefOrRefEncoded ().GetType ();
    }

    Type* TypeSigElem::GetValueType () const
    {
        assert ( GetElemType () == ELEMENT_TYPE_VALUETYPE );
        SigElemStream stream ( this, true );
        return stream.ReadTypeDefOrRefEncoded ().GetType ();
    }

}
