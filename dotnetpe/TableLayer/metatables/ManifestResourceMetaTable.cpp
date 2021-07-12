#include "StdInc.h"

namespace dotnetpe
{

    ManifestResourceMetaTable::ManifestResourceMetaTable ()
    {

    }

    ManifestResourceMetaTable::~ManifestResourceMetaTable ()
    {

    }

    void ManifestResourceMetaTable::Init ( TableLayer* pTableLayer )
    {
        MetaTableBase < ManifestResourceMetaTable, _ManifestResourceMetaRecord >::Init ( pTableLayer );

        m_iRecordSize += m_Offset.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Flags.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Name.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Implementation.Init ( pTableLayer, m_iRecordSize );
    }

    _ManifestResourceMetaRecord::_ManifestResourceMetaRecord ( ManifestResourceMetaTable* pTable, void* pRecord )
        : MetaRecordBase < ManifestResourceMetaTable > ( pTable, pRecord )
    {

    }

    dword _ManifestResourceMetaRecord::GetOffset () const
    {
        return m_pTable->m_Offset.Read ( m_pRecord );
    }

    dword _ManifestResourceMetaRecord::GetFlags () const
    {
        return m_pTable->m_Flags.Read ( m_pRecord );
    }

    const char* _ManifestResourceMetaRecord::GetName () const
    {
        return m_pTable->m_Name.Read ( m_pRecord );
    }

    dword _ManifestResourceMetaRecord::GetImplementation ( MetaTable*& pTable ) const
    {
        return m_pTable->m_Implementation.Read ( m_pRecord, pTable );
    }

}
