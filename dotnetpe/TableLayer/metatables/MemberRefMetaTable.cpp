#include "StdInc.h"

namespace dotnetpe
{

    MemberRefMetaTable::MemberRefMetaTable ()
    {

    }

    MemberRefMetaTable::~MemberRefMetaTable ()
    {

    }

    void MemberRefMetaTable::Init ( TableLayer* pTableLayer )
    {
        MetaTableBase < MemberRefMetaTable, _MemberRefMetaRecord >::Init ( pTableLayer );

        m_iRecordSize += m_Class.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Name.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Signature.Init ( pTableLayer, m_iRecordSize );
    }

    _MemberRefMetaRecord::_MemberRefMetaRecord ( MemberRefMetaTable* pTable, void* pRecord )
        : MetaRecordBase < MemberRefMetaTable > ( pTable, pRecord )
    {
        
    }

    dword _MemberRefMetaRecord::GetClass ( MetaTable*& pTable ) const
    {
        return m_pTable->m_Class.Read ( m_pRecord, pTable );
    }

    const char* _MemberRefMetaRecord::GetName () const
    {
        return m_pTable->m_Name.Read ( m_pRecord );
    }

    const void* _MemberRefMetaRecord::GetSignature () const
    {
        return m_pTable->m_Signature.Read ( m_pRecord );
    }

}
