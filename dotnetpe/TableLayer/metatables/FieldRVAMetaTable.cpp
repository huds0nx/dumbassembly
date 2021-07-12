#include "StdInc.h"

namespace dotnetpe
{

    FieldRVAMetaTable::FieldRVAMetaTable ()
    {

    }

    FieldRVAMetaTable::~FieldRVAMetaTable ()
    {

    }

    void FieldRVAMetaTable::Init ( TableLayer* pTableLayer )
    {
        MetaTableBase < FieldRVAMetaTable, _FieldRVAMetaRecord >::Init ( pTableLayer );

        m_iRecordSize += m_RVA.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Field.Init ( pTableLayer, m_iRecordSize );
    }

    _FieldRVAMetaRecord::_FieldRVAMetaRecord ( FieldRVAMetaTable* pTable, void* pRecord )
        : MetaRecordBase < FieldRVAMetaTable > ( pTable, pRecord )
    {

    }

    dword _FieldRVAMetaRecord::GetRVA () const
    {
        return m_pTable->m_RVA.Read ( m_pRecord );
    }

    dword _FieldRVAMetaRecord::GetField () const
    {
        return m_pTable->m_Field.Read ( m_pRecord );
    }

}
