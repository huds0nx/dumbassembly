#include "StdInc.h"

namespace dotnetpe
{

    MethodRef::MethodRef ()
    {

    }

    MethodRef::~MethodRef ()
    {

    }

    TypeRef* MethodRef::GetType () const
    {
        MetaTable* pTable = NULL;
        dword dwIndex = m_Record.GetClass ( pTable );
        if ( !dwIndex )
            return NULL;

        if ( pTable->GetType () == MetaTable::METATABLE_TYPEREF )
            return &m_pStructureLayer->m_TypeRefs [ dwIndex - 1 ];

        return NULL;
    }

}
