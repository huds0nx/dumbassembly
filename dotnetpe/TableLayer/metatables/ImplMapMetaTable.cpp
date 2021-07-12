#include "StdInc.h"

namespace dotnetpe
{

    ImplMapMetaTable::ImplMapMetaTable ()
    {

    }

    ImplMapMetaTable::~ImplMapMetaTable ()
    {

    }

    void ImplMapMetaTable::Init ( TableLayer* pTableLayer )
    {
        MetaTableBase < ImplMapMetaTable, _ImplMapMetaRecord >::Init ( pTableLayer );

        m_iRecordSize += m_MappingFlags.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_MemberForwarded.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_ImportName.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_ImportScope.Init ( pTableLayer, m_iRecordSize );
    }

    _ImplMapMetaRecord::_ImplMapMetaRecord ( ImplMapMetaTable* pTable, void* pRecord )
        : MetaRecordBase < ImplMapMetaTable > ( pTable, pRecord )
    {

    }

    word _ImplMapMetaRecord::GetMappingFlags () const
    {
        return m_pTable->m_MappingFlags.Read ( m_pRecord );
    }

    dword _ImplMapMetaRecord::GetMemberForwarded ( MetaTable*& pTable ) const
    {
        return m_pTable->m_MemberForwarded.Read ( m_pRecord, pTable );
    }

    const char* _ImplMapMetaRecord::GetImportName () const
    {
        return m_pTable->m_ImportName.Read ( m_pRecord );
    }

    dword _ImplMapMetaRecord::GetImportScope () const
    {
        return m_pTable->m_ImportScope.Read ( m_pRecord );
    }

}
