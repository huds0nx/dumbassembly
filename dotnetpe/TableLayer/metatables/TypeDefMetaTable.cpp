#include "StdInc.h"

namespace dotnetpe
{

    TypeDefMetaTable::TypeDefMetaTable ()
    {

    }

    TypeDefMetaTable::~TypeDefMetaTable ()
    {
        
    }

    void TypeDefMetaTable::Init ( TableLayer* pTableLayer )
    {
        MetaTableBase < TypeDefMetaTable, _TypeDefMetaRecord >::Init ( pTableLayer );

        m_iRecordSize += m_Flags.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_TypeName.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_TypeNamespace.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Extends.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_FieldList.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_MethodList.Init ( pTableLayer, m_iRecordSize );
    }

    _TypeDefMetaRecord::_TypeDefMetaRecord ( TypeDefMetaTable* pTable, void* pRecord )
        : MetaRecordBase < TypeDefMetaTable > ( pTable, pRecord )
    {
        
    }

    CorTypeAttr _TypeDefMetaRecord::GetFlags () const
    {
        return (CorTypeAttr)m_pTable->m_Flags.Read ( m_pRecord );
    }

    const char* _TypeDefMetaRecord::GetName () const
    {
        return m_pTable->m_TypeName.Read ( m_pRecord );
    }

    const char* _TypeDefMetaRecord::GetNamespace () const
    {
        return m_pTable->m_TypeNamespace.Read ( m_pRecord );
    }

    dword _TypeDefMetaRecord::GetExtends ( MetaTable*& pTable ) const
    {
        return m_pTable->m_Extends.Read ( m_pRecord, pTable );
    }

    dword _TypeDefMetaRecord::GetFieldList () const
    {
        return m_pTable->m_FieldList.Read ( m_pRecord );
    }

    dword _TypeDefMetaRecord::GetMethodList () const
    {
        return m_pTable->m_MethodList.Read ( m_pRecord );
    }

}
