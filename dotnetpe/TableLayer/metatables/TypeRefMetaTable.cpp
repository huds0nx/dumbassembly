#include "StdInc.h"

namespace dotnetpe
{

    TypeRefMetaTable::TypeRefMetaTable ()
    {

    }

    TypeRefMetaTable::~TypeRefMetaTable ()
    {

    }

    void TypeRefMetaTable::Init ( TableLayer* pTableLayer )
    {
        MetaTableBase < TypeRefMetaTable, _TypeRefMetaRecord >::Init ( pTableLayer );

        m_iRecordSize += m_ResolutionScope.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_TypeName.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_TypeNamespace.Init ( pTableLayer, m_iRecordSize );
    }

    _TypeRefMetaRecord::_TypeRefMetaRecord ( TypeRefMetaTable* pTable, void* pRecord )
        : MetaRecordBase < TypeRefMetaTable > ( pTable, pRecord )
    {

    }

    dword _TypeRefMetaRecord::GetResolutionScope ( MetaTable*& pTable ) const
    {
        return m_pTable->m_ResolutionScope.Read ( m_pRecord, pTable );
    }

    const char* _TypeRefMetaRecord::GetName () const
    {
        return m_pTable->m_TypeName.Read ( m_pRecord );
    }

    const char* _TypeRefMetaRecord::GetNamespace () const
    {
        return m_pTable->m_TypeNamespace.Read ( m_pRecord );
    }

}
