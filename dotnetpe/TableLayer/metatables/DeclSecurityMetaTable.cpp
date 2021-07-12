#include "StdInc.h"

namespace dotnetpe
{

    DeclSecurityMetaTable::DeclSecurityMetaTable ()
    {

    }

    DeclSecurityMetaTable::~DeclSecurityMetaTable ()
    {

    }

    void DeclSecurityMetaTable::Init ( TableLayer* pTableLayer )
    {
        MetaTableBase < DeclSecurityMetaTable, _DeclSecurityMetaRecord >::Init ( pTableLayer );

        m_iRecordSize += m_Action.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Parent.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_PermissionSet.Init ( pTableLayer, m_iRecordSize );
    }

    _DeclSecurityMetaRecord::_DeclSecurityMetaRecord ( DeclSecurityMetaTable* pTable, void* pRecord )
        : MetaRecordBase < DeclSecurityMetaTable > ( pTable, pRecord )
    {

    }

    word _DeclSecurityMetaRecord::GetAction () const
    {
        return m_pTable->m_Action.Read ( m_pRecord );
    }

    dword _DeclSecurityMetaRecord::GetParent ( MetaTable*& pTable ) const
    {
        return m_pTable->m_Parent.Read ( m_pRecord, pTable );
    }

    const void* _DeclSecurityMetaRecord::GetPermissionSet () const
    {
        return m_pTable->m_PermissionSet.Read ( m_pRecord );
    }

}
