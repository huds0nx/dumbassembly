#include "StdInc.h"

namespace dotnetpe
{

    CustomAttributeMetaTable::CustomAttributeMetaTable ()
    {

    }

    CustomAttributeMetaTable::~CustomAttributeMetaTable ()
    {

    }

    void CustomAttributeMetaTable::Init ( TableLayer* pTableLayer )
    {
        MetaTableBase < CustomAttributeMetaTable, _CustomAttributeMetaRecord >::Init ( pTableLayer );

        m_iRecordSize += m_Parent.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Type.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Value.Init ( pTableLayer, m_iRecordSize );
    }

    _CustomAttributeMetaRecord::_CustomAttributeMetaRecord ( CustomAttributeMetaTable* pTable, void* pRecord )
        : MetaRecordBase < CustomAttributeMetaTable > ( pTable, pRecord )
    {

    }

    dword _CustomAttributeMetaRecord::GetParent ( MetaTable*& pTable ) const
    {
        return m_pTable->m_Parent.Read ( m_pRecord, pTable );
    }

    dword _CustomAttributeMetaRecord::GetType ( MetaTable*& pTable ) const
    {
        return m_pTable->m_Type.Read ( m_pRecord, pTable );
    }

    const void* _CustomAttributeMetaRecord::GetValue () const
    {
        return m_pTable->m_Value.Read ( m_pRecord );
    }

}
