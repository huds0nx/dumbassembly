#include "StdInc.h"

namespace dotnetpe
{

    MethodSemanticsMetaTable::MethodSemanticsMetaTable ()
    {

    }

    MethodSemanticsMetaTable::~MethodSemanticsMetaTable ()
    {

    }

    void MethodSemanticsMetaTable::Init ( TableLayer* pTableLayer )
    {
        MetaTableBase < MethodSemanticsMetaTable, _MethodSemanticsMetaRecord >::Init ( pTableLayer );

        m_iRecordSize += m_Semantics.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Method.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Association.Init ( pTableLayer, m_iRecordSize );
    }

    _MethodSemanticsMetaRecord::_MethodSemanticsMetaRecord ( MethodSemanticsMetaTable* pTable, void* pRecord )
        : MetaRecordBase < MethodSemanticsMetaTable > ( pTable, pRecord )
    {

    }

    word _MethodSemanticsMetaRecord::GetSemantics () const
    {
        return m_pTable->m_Semantics.Read ( m_pRecord );
    }

    dword _MethodSemanticsMetaRecord::GetMethod () const
    {
        return m_pTable->m_Method.Read ( m_pRecord );
    }

    dword _MethodSemanticsMetaRecord::GetAssociation ( MetaTable*& pTable ) const
    {
        return m_pTable->m_Association.Read ( m_pRecord, pTable );
    }

}
