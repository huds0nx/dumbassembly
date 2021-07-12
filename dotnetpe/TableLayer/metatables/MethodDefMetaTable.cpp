#include "StdInc.h"

namespace dotnetpe
{

    MethodDefMetaTable::MethodDefMetaTable ()
    {

    }

    MethodDefMetaTable::~MethodDefMetaTable ()
    {

    }

    void MethodDefMetaTable::Init ( TableLayer* pTableLayer )
    {
        MetaTableBase < MethodDefMetaTable, _MethodDefMetaRecord >::Init ( pTableLayer );

        m_iRecordSize += m_RVA.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_ImplFlags.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Flags.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Name.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Signature.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_ParamList.Init ( pTableLayer, m_iRecordSize );
    }

    _MethodDefMetaRecord::_MethodDefMetaRecord ( MethodDefMetaTable* pTable, void* pRecord )
        : MetaRecordBase < MethodDefMetaTable > ( pTable, pRecord )
    {

    }

    dword _MethodDefMetaRecord::GetRVA () const
    {
        return m_pTable->m_RVA.Read ( m_pRecord );
    }

    word _MethodDefMetaRecord::GetImplFlags () const
    {
        return m_pTable->m_ImplFlags.Read ( m_pRecord );
    }

    word _MethodDefMetaRecord::GetFlags () const
    {
        return m_pTable->m_Flags.Read ( m_pRecord );
    }

    const char* _MethodDefMetaRecord::GetName () const
    {
        return m_pTable->m_Name.Read ( m_pRecord );
    }

    const void* _MethodDefMetaRecord::GetSignature () const
    {
        return m_pTable->m_Signature.Read ( m_pRecord );
    }

    dword _MethodDefMetaRecord::GetParamList () const
    {
        return m_pTable->m_ParamList.Read ( m_pRecord );
    }

}
