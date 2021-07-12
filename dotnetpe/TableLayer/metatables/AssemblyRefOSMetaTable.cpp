#include "StdInc.h"

namespace dotnetpe
{

    AssemblyRefOSMetaTable::AssemblyRefOSMetaTable ()
    {

    }

    AssemblyRefOSMetaTable::~AssemblyRefOSMetaTable ()
    {

    }

    void AssemblyRefOSMetaTable::Init ( TableLayer* pTableLayer )
    {
        MetaTableBase < AssemblyRefOSMetaTable, _AssemblyRefOSMetaRecord >::Init ( pTableLayer );

        m_iRecordSize += m_OSPlatformId.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_OSMajorVersion.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_OSMinorVersion.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_AssemblyRef.Init ( pTableLayer, m_iRecordSize );
    }

    _AssemblyRefOSMetaRecord::_AssemblyRefOSMetaRecord ( AssemblyRefOSMetaTable* pTable, void* pRecord )
        : MetaRecordBase < AssemblyRefOSMetaTable > ( pTable, pRecord )
    {

    }

    dword _AssemblyRefOSMetaRecord::GetOSPlatformId () const
    {
        return m_pTable->m_OSPlatformId.Read ( m_pRecord );
    }

    dword _AssemblyRefOSMetaRecord::GetOSMajorVersion () const
    {
        return m_pTable->m_OSMajorVersion.Read ( m_pRecord );
    }

    dword _AssemblyRefOSMetaRecord::GetOSMinorVersion () const
    {
        return m_pTable->m_OSMinorVersion.Read ( m_pRecord );
    }

    dword _AssemblyRefOSMetaRecord::GetAssemblyRef () const
    {
        return m_pTable->m_AssemblyRef.Read ( m_pRecord );
    }

}
