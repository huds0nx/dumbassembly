#include "StdInc.h"

namespace dotnetpe
{

    FieldLayoutMetaTable::FieldLayoutMetaTable ()
    {

    }

    FieldLayoutMetaTable::~FieldLayoutMetaTable ()
    {

    }

    void FieldLayoutMetaTable::Init ( TableLayer* pTableLayer )
    {
        MetaTableBase < FieldLayoutMetaTable, _FieldLayoutMetaRecord >::Init ( pTableLayer );

        m_iRecordSize += m_Offset.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Field.Init ( pTableLayer, m_iRecordSize );
    }

    _FieldLayoutMetaRecord::_FieldLayoutMetaRecord ( FieldLayoutMetaTable* pTable, void* pRecord )
        : MetaRecordBase < FieldLayoutMetaTable > ( pTable, pRecord )
    {

    }

    dword _FieldLayoutMetaRecord::GetOffset () const
    {
        return m_pTable->m_Offset.Read ( m_pRecord );
    }

    dword _FieldLayoutMetaRecord::GetField () const
    {
        return m_pTable->m_Field.Read ( m_pRecord );
    }

}
