#include "StdInc.h"

namespace dotnetpe
{

    MethodImplMetaTable::MethodImplMetaTable ()
    {

    }

    MethodImplMetaTable::~MethodImplMetaTable ()
    {

    }

    void MethodImplMetaTable::Init ( TableLayer* pTableLayer )
    {
        MetaTableBase < MethodImplMetaTable, _MethodImplMetaRecord >::Init ( pTableLayer );

        m_iRecordSize += m_Class.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_MethodBody.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_MethodDeclaration.Init ( pTableLayer, m_iRecordSize );
    }

    _MethodImplMetaRecord::_MethodImplMetaRecord ( MethodImplMetaTable* pTable, void* pRecord )
        : MetaRecordBase < MethodImplMetaTable > ( pTable, pRecord )
    {

    }

    dword _MethodImplMetaRecord::GetClass () const
    {
        return m_pTable->m_Class.Read ( m_pRecord );
    }

    dword _MethodImplMetaRecord::GetMethodBody ( MetaTable*& pTable ) const
    {
        return m_pTable->m_MethodBody.Read ( m_pRecord, pTable );
    }

    dword _MethodImplMetaRecord::GetMethodDeclaration ( MetaTable*& pTable ) const
    {
        return m_pTable->m_MethodDeclaration.Read ( m_pRecord, pTable );
    }

}
