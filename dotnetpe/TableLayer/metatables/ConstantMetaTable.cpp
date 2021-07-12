#include "StdInc.h"

namespace dotnetpe
{

    ConstantMetaTable::ConstantMetaTable ()
    {

    }

    ConstantMetaTable::~ConstantMetaTable ()
    {

    }

    void ConstantMetaTable::Init ( TableLayer* pTableLayer )
    {
        MetaTableBase < ConstantMetaTable, _ConstantMetaRecord >::Init ( pTableLayer );

        m_iRecordSize += m_Type.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Pad0.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Parent.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Value.Init ( pTableLayer, m_iRecordSize );
    }

    _ConstantMetaRecord::_ConstantMetaRecord ( ConstantMetaTable* pTable, void* pRecord )
        : MetaRecordBase < ConstantMetaTable > ( pTable, pRecord )
    {

    }

    byte _ConstantMetaRecord::GetType () const
    {
        return m_pTable->m_Type.Read ( m_pRecord );
    }

    dword _ConstantMetaRecord::GetParent ( MetaTable*& pTable ) const
    {
        return m_pTable->m_Parent.Read ( m_pRecord, pTable );
    }

    const void* _ConstantMetaRecord::GetValue () const
    {
        return m_pTable->m_Value.Read ( m_pRecord );
    }

}
