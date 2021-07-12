#include "StdInc.h"

namespace dotnetpe
{

    AssemblyRefMetaTable::AssemblyRefMetaTable ()
    {

    }

    AssemblyRefMetaTable::~AssemblyRefMetaTable ()
    {

    }

    void AssemblyRefMetaTable::Init ( TableLayer* pTableLayer )
    {
        MetaTableBase < AssemblyRefMetaTable, _AssemblyRefMetaRecord >::Init ( pTableLayer );

        m_iRecordSize += m_MajorVersion.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_MinorVersion.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_BuildNumber.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_RevisionNumber.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Flags.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_PublicKeyOrToken.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Name.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Culture.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_HashValue.Init ( pTableLayer, m_iRecordSize );
    }

    _AssemblyRefMetaRecord::_AssemblyRefMetaRecord ( AssemblyRefMetaTable* pTable, void* pRecord )
        : MetaRecordBase < AssemblyRefMetaTable > ( pTable, pRecord )
    {

    }

    word _AssemblyRefMetaRecord::GetMajorVersion () const
    {
        return m_pTable->m_MajorVersion.Read ( m_pRecord );
    }

    word _AssemblyRefMetaRecord::GetMinorVersion () const
    {
        return m_pTable->m_MinorVersion.Read ( m_pRecord );
    }

    word _AssemblyRefMetaRecord::GetBuildNumber () const
    {
        return m_pTable->m_BuildNumber.Read ( m_pRecord );
    }

    word _AssemblyRefMetaRecord::GetRevisionNumber () const
    {
        return m_pTable->m_RevisionNumber.Read ( m_pRecord );
    }

    dword _AssemblyRefMetaRecord::GetFlags () const
    {
        return m_pTable->m_Flags.Read ( m_pRecord );
    }

    const void* _AssemblyRefMetaRecord::GetPublicKeyOrToken () const
    {
        return m_pTable->m_PublicKeyOrToken.Read ( m_pRecord );
    }

    const char* _AssemblyRefMetaRecord::GetName () const
    {
        return m_pTable->m_Name.Read ( m_pRecord );
    }

    const char* _AssemblyRefMetaRecord::GetCulture () const
    {
        return m_pTable->m_Culture.Read ( m_pRecord );
    }

    const void* _AssemblyRefMetaRecord::GetHashValue () const
    {
        return m_pTable->m_HashValue.Read ( m_pRecord );
    }

}
