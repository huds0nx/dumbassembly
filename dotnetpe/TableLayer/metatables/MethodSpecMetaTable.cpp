#include "StdInc.h"

namespace dotnetpe
{

    MethodSpecMetaTable::MethodSpecMetaTable ()
    {

    }

    MethodSpecMetaTable::~MethodSpecMetaTable ()
    {

    }

    void MethodSpecMetaTable::Init ( TableLayer* pTableLayer )
    {
        MetaTableBase < MethodSpecMetaTable, _MethodSpecMetaRecord >::Init ( pTableLayer );

        m_iRecordSize += m_Method.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Signature.Init ( pTableLayer, m_iRecordSize );
    }

    _MethodSpecMetaRecord::_MethodSpecMetaRecord ( MethodSpecMetaTable* pTable, void* pRecord )
        : MetaRecordBase < MethodSpecMetaTable > ( pTable, pRecord )
    {

    }

    dword _MethodSpecMetaRecord::GetMethod ( MetaTable*& pTable ) const
    {
        return m_pTable->m_Method.Read ( m_pRecord, pTable );
    }

    const void* _MethodSpecMetaRecord::GetSignature () const
    {
        return m_pTable->m_Signature.Read ( m_pRecord );
    }

}
