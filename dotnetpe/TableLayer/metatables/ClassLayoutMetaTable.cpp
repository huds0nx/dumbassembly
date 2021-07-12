#include "StdInc.h"

namespace dotnetpe
{

    ClassLayoutMetaTable::ClassLayoutMetaTable ()
    {

    }

    ClassLayoutMetaTable::~ClassLayoutMetaTable ()
    {

    }

    void ClassLayoutMetaTable::Init ( TableLayer* pTableLayer )
    {
        MetaTableBase < ClassLayoutMetaTable, _ClassLayoutMetaRecord >::Init ( pTableLayer );

        m_iRecordSize += m_PackingSize.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_ClassSize.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Parent.Init ( pTableLayer, m_iRecordSize );
    }

    _ClassLayoutMetaRecord::_ClassLayoutMetaRecord ( ClassLayoutMetaTable* pTable, void* pRecord )
        : MetaRecordBase < ClassLayoutMetaTable > ( pTable, pRecord )
    {

    }

    word _ClassLayoutMetaRecord::GetPackingSize () const
    {
        return m_pTable->m_PackingSize.Read ( m_pRecord );
    }

    dword _ClassLayoutMetaRecord::GetClassSize () const
    {
        return m_pTable->m_ClassSize.Read ( m_pRecord );
    }

    dword _ClassLayoutMetaRecord::GetParent () const
    {
        return m_pTable->m_Parent.Read ( m_pRecord );
    }

}
