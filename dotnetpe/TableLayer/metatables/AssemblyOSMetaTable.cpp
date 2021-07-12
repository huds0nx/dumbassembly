#include "StdInc.h"

namespace dotnetpe
{

    AssemblyOSMetaTable::AssemblyOSMetaTable ()
    {

    }

    AssemblyOSMetaTable::~AssemblyOSMetaTable ()
    {

    }

    void AssemblyOSMetaTable::Init ( TableLayer* pTableLayer )
    {
        MetaTableBase < AssemblyOSMetaTable, _AssemblyOSMetaRecord >::Init ( pTableLayer );

        m_iRecordSize += m_OSPlatformID.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_OSMajorVersion.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_OSMinorVersion.Init ( pTableLayer, m_iRecordSize );
    }

    _AssemblyOSMetaRecord::_AssemblyOSMetaRecord ( AssemblyOSMetaTable* pTable, void* pRecord )
        : MetaRecordBase < AssemblyOSMetaTable > ( pTable, pRecord )
    {

    }

    dword _AssemblyOSMetaRecord::GetOSPlatformID () const
    {
        return m_pTable->m_OSPlatformID.Read ( m_pRecord );
    }

    dword _AssemblyOSMetaRecord::GetOSMajorVersion () const
    {
        return m_pTable->m_OSMajorVersion.Read ( m_pRecord );
    }

    dword _AssemblyOSMetaRecord::GetOSMinorVersion () const
    {
        return m_pTable->m_OSMinorVersion.Read ( m_pRecord );
    }

}

