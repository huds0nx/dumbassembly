#include "StdInc.h"

namespace dotnetpe
{

    NestedClassMetaTable::NestedClassMetaTable ()
    {

    }

    NestedClassMetaTable::~NestedClassMetaTable ()
    {

    }

    void NestedClassMetaTable::Init ( TableLayer* pTableLayer )
    {
        MetaTableBase < NestedClassMetaTable, _NestedClassMetaRecord >::Init ( pTableLayer );

        m_iRecordSize += m_NestedClass.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_EnclosingClass.Init ( pTableLayer, m_iRecordSize );
    }

    _NestedClassMetaRecord::_NestedClassMetaRecord ( NestedClassMetaTable* pTable, void* pRecord )
        : MetaRecordBase < NestedClassMetaTable > ( pTable, pRecord )
    {

    }

    dword _NestedClassMetaRecord::GetNestedClass () const
    {
        return m_pTable->m_NestedClass.Read ( m_pRecord );
    }

    dword _NestedClassMetaRecord::GetEnclosingClass () const
    {
        return m_pTable->m_EnclosingClass.Read ( m_pRecord );
    }

}
