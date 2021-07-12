#include "StdInc.h"

namespace dotnetpe
{

    CorElementType Field::GetTypeKind () const
    {
        FieldSig sig ( m_pStructureLayer, m_Record.GetSignature () );
        return sig.GetTypeSigElem ().GetElemType ();
    }

    Type* Field::GetClass () const
    {
        FieldSig sig ( m_pStructureLayer, m_Record.GetSignature () );

        if ( GetTypeKind () == ELEMENT_TYPE_VALUETYPE )
            return sig.GetTypeSigElem ().GetValueType ();
        else if ( GetTypeKind () == ELEMENT_TYPE_CLASS )
            return sig.GetTypeSigElem ().GetClass ();
        else
            return NULL;
    }

    void* Field::GetData () const
    {
        if ( !m_Record.GetFlags () & fdHasFieldRVA )
            return NULL;

        FieldRVAMetaTable* pRVATable = (FieldRVAMetaTable *)GetTableLayer ()->
            GetMetaTable ( MetaTable::METATABLE_FIELDRVA );
        for ( int i = 1; i <= pRVATable->GetNumRecords (); i++ )
        {
            if ( pRVATable->GetRecord ( i ).GetField () == (Token() & 0x00FFFFFF) )
                return GetPE ()->RVAToPtr ( pRVATable->GetRecord ( i ).GetRVA () );
        }
        return NULL;
    }

    Field::eVisibility Field::GetVisibility () const
    {
        switch ( m_Record.GetFlags () & fdFieldAccessMask )
        {
            case fdPrivateScope:
            case fdPrivate:
                return PRIVATE;

            case fdFamANDAssem:
            case fdFamily:
                return PROTECTED;

            case fdFamORAssem:
            case fdAssembly:
                return INTERNAL;

            case fdPublic:
                return PUBLIC;
        }
        return PRIVATE;
    }

    bool Field::IsStatic () const
    {
        return ( m_Record.GetFlags () & fdStatic ) != 0;
    }

}
