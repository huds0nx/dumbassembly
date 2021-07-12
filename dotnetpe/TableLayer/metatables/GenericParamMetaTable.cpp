#include "StdInc.h"

namespace dotnetpe
{

    GenericParamMetaTable::GenericParamMetaTable ()
    {

    }

    GenericParamMetaTable::~GenericParamMetaTable ()
    {

    }

    void GenericParamMetaTable::Init ( TableLayer* pTableLayer )
    {
        MetaTableBase < GenericParamMetaTable, _GenericParamMetaRecord >::Init ( pTableLayer );

        m_iRecordSize += m_Number.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Flags.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Owner.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Name.Init ( pTableLayer, m_iRecordSize );
    }

    _GenericParamMetaRecord::_GenericParamMetaRecord ( GenericParamMetaTable* pTable, void* pRecord )
        : MetaRecordBase < GenericParamMetaTable > ( pTable, pRecord )
    {

    }

    word _GenericParamMetaRecord::GetNumber () const
    {
        return m_pTable->m_Number.Read ( m_pRecord );
    }

    word _GenericParamMetaRecord::GetFlags () const
    {
        return m_pTable->m_Flags.Read ( m_pRecord );
    }

    dword _GenericParamMetaRecord::GetOwner ( MetaTable*& pTable ) const
    {
        return m_pTable->m_Owner.Read ( m_pRecord, pTable );
    }

    const char* _GenericParamMetaRecord::GetName () const
    {
        return m_pTable->m_Name.Read ( m_pRecord );
    }

}
