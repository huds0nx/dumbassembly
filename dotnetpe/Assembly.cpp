#include "StdInc.h"

namespace dotnetpe
{

    const void* Assembly::GetPublicKey () const
    {
        return m_Record.GetPublicKey ();
    }

    bool Assembly::GetPublicKeyToken ( byte* pToken ) const
    {
        if ( !GetPublicKey () )
            return false;

        MemoryStream publicKeyStream ( GetPublicKey (), 0x7FFFFFFF, true );
        dword dwPublicKeyLength = publicKeyStream.ReadVarUInt32 ();

        byte* pPublicKeyToken = NULL;
        dword dwPublicKeyTokenLength = 0;
        if ( !StrongNameTokenFromPublicKey (
                (byte *)publicKeyStream.Data () + publicKeyStream.GetPosition (),
                dwPublicKeyLength,
                &pPublicKeyToken,
                &dwPublicKeyTokenLength ) )
        {
            return false;
        }
        memcpy ( pToken, pPublicKeyToken, dwPublicKeyTokenLength );
        StrongNameFreeBuffer ( pPublicKeyToken );
        return true;
    }

	const void* Assembly::GetCustomAttributeData ( const char* pszAttributeTypeName ) const
	{
		TypeDef* pAttrType = m_pStructureLayer->GetModule ()->GetType ( pszAttributeTypeName );
		if ( !pAttrType )
			return NULL;

		MethodDef* pAttrCtor = pAttrType->GetMethod ( ".ctor" );
		if ( !pAttrCtor )
			return NULL;

		CustomAttributeMetaTable* pAttrMetaTable = (CustomAttributeMetaTable *)GetTableLayer ()->GetMetaTable ( MetaTable::METATABLE_CUSTOMATTRIBUTE );
		for ( dword i = 1; i <= pAttrMetaTable->GetNumRecords (); i++ )
		{
			CustomAttributeMetaTable::Record rec = pAttrMetaTable->GetRecord ( i );
			MetaTable* pRecTable = NULL;
			dword dwParentIdx = rec.GetParent ( pRecTable );
			if ( pRecTable->GetType () != MetaTable::METATABLE_ASSEMBLY || dwParentIdx != m_Record.GetIndex () )
            {
				continue;
            }

			dword dwTypeIdx = rec.GetType ( pRecTable );
			if ( pRecTable->GetType () != MetaTable::METATABLE_METHODDEF ||
                 ((pRecTable->GetType () << 24) | dwTypeIdx) != pAttrCtor->Token () )
            {
				continue;
            }

	        return rec.GetValue ();
		}

		return NULL;
	}

}
