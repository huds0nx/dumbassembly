#include "StdInc.h"

namespace dotnetpe
{

    FieldMarshalMetaTable::FieldMarshalMetaTable ()
    {

    }

    FieldMarshalMetaTable::~FieldMarshalMetaTable ()
    {

    }

    void FieldMarshalMetaTable::Init ( TableLayer* pTableLayer )
    {
        MetaTableBase < FieldMarshalMetaTable, _FieldMarshalMetaRecord >::Init ( pTableLayer );

        m_iRecordSize += m_Parent.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_NativeType.Init ( pTableLayer, m_iRecordSize );
    }

    _FieldMarshalMetaRecord::_FieldMarshalMetaRecord ( FieldMarshalMetaTable* pTable, void* pRecord )
        : MetaRecordBase < FieldMarshalMetaTable > ( pTable, pRecord )
    {
        
    }

    dword _FieldMarshalMetaRecord::GetParent ( MetaTable*& pTable ) const
    {
        return m_pTable->m_Parent.Read ( m_pRecord, pTable );
    }

    const void* _FieldMarshalMetaRecord::GetNativeType () const
    {
        return m_pTable->m_NativeType.Read ( m_pRecord );
    }

}
