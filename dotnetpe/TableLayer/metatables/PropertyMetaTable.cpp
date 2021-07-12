#include "StdInc.h"

namespace dotnetpe
{

    PropertyMetaTable::PropertyMetaTable ()
    {

    }

    PropertyMetaTable::~PropertyMetaTable ()
    {

    }

    void PropertyMetaTable::Init ( TableLayer* pTableLayer )
    {
        MetaTableBase < PropertyMetaTable, _PropertyMetaRecord >::Init ( pTableLayer );

        m_iRecordSize += m_Flags.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Name.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Type.Init ( pTableLayer, m_iRecordSize );
    }

    _PropertyMetaRecord::_PropertyMetaRecord ( PropertyMetaTable* pTable, void* pRecord )
        : MetaRecordBase < PropertyMetaTable > ( pTable, pRecord )
    {

    }

    word _PropertyMetaRecord::GetFlags () const
    {
        return m_pTable->m_Flags.Read ( m_pRecord );
    }

    const char* _PropertyMetaRecord::GetName () const
    {
        return m_pTable->m_Name.Read ( m_pRecord );
    }

    const void* _PropertyMetaRecord::GetType () const
    {
        return m_pTable->m_Type.Read ( m_pRecord );
    }

}
