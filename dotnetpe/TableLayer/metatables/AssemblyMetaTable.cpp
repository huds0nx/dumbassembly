#include "StdInc.h"

namespace dotnetpe
{

    AssemblyMetaTable::AssemblyMetaTable ()
    {

    }

    AssemblyMetaTable::~AssemblyMetaTable ()
    {
        
    }

    void AssemblyMetaTable::Init ( TableLayer* pTableLayer )
    {
        MetaTableBase < AssemblyMetaTable, _AssemblyMetaRecord >::Init ( pTableLayer );

        m_iRecordSize += m_HashAlgId.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_MajorVersion.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_MinorVersion.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_BuildNumber.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_RevisionNumber.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Flags.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_PublicKey.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Name.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Culture.Init ( pTableLayer, m_iRecordSize );
    }

    _AssemblyMetaRecord::_AssemblyMetaRecord ( AssemblyMetaTable* pTable, void* pRecord )
        : MetaRecordBase < AssemblyMetaTable > ( pTable, pRecord )
    {

    }

    dword _AssemblyMetaRecord::GetHashAlgId () const
    {
        return m_pTable->m_HashAlgId.Read ( m_pRecord );
    }

    word _AssemblyMetaRecord::GetMajorVersion () const
    {
        return m_pTable->m_MajorVersion.Read ( m_pRecord );
    }

    word _AssemblyMetaRecord::GetMinorVersion () const
    {
        return m_pTable->m_MinorVersion.Read ( m_pRecord );
    }

    word _AssemblyMetaRecord::GetBuildNumber () const
    {
        return m_pTable->m_BuildNumber.Read ( m_pRecord );
    }

    word _AssemblyMetaRecord::GetRevisionNumber () const
    {
        return m_pTable->m_RevisionNumber.Read ( m_pRecord );
    }

    dword _AssemblyMetaRecord::GetFlags () const
    {
        return m_pTable->m_Flags.Read ( m_pRecord );
    }

    const void* _AssemblyMetaRecord::GetPublicKey () const
    {
        return m_pTable->m_PublicKey.Read ( m_pRecord );
    }

    const char* _AssemblyMetaRecord::GetName () const
    {
        return m_pTable->m_Name.Read ( m_pRecord );
    }

    const char* _AssemblyMetaRecord::GetCulture () const
    {
        return m_pTable->m_Culture.Read ( m_pRecord );
    }

}
