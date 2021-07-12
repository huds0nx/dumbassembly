#include "StdInc.h"

namespace dotnetpe
{

    FieldMetaTable::FieldMetaTable ()
    {

    }

    FieldMetaTable::~FieldMetaTable ()
    {

    }

    void FieldMetaTable::Init ( TableLayer* pTableLayer )
    {
        MetaTableBase < FieldMetaTable, _FieldMetaRecord >::Init ( pTableLayer );

        m_iRecordSize += m_Flags.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Name.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Signature.Init ( pTableLayer, m_iRecordSize );
    }

    _FieldMetaRecord::_FieldMetaRecord ( FieldMetaTable* pTable, void* pRecord )
        : MetaRecordBase < FieldMetaTable > ( pTable, pRecord )
    {

    }

    word _FieldMetaRecord::GetFlags () const
    {
        return m_pTable->m_Flags.Read ( m_pRecord );
    }

    const char* _FieldMetaRecord::GetName () const
    {
        return m_pTable->m_Name.Read ( m_pRecord );
    }

    const void* _FieldMetaRecord::GetSignature () const
    {
        return m_pTable->m_Signature.Read ( m_pRecord );
    }

}
