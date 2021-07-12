#include "StdInc.h"

namespace dotnetpe
{

    ParamMetaTable::ParamMetaTable ()
    {

    }

    ParamMetaTable::~ParamMetaTable ()
    {

    }

    void ParamMetaTable::Init ( TableLayer* pTableLayer )
    {
        MetaTableBase < ParamMetaTable, _ParamMetaRecord >::Init ( pTableLayer );

        m_iRecordSize += m_Flags.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Sequence.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Name.Init ( pTableLayer, m_iRecordSize );
    }

    _ParamMetaRecord::_ParamMetaRecord ( ParamMetaTable* pTable, void* pRecord )
        : MetaRecordBase < ParamMetaTable > ( pTable, pRecord )
    {
        
    }

    word _ParamMetaRecord::GetFlags () const
    {
        return m_pTable->m_Flags.Read ( m_pRecord );
    }

    word _ParamMetaRecord::GetSequence () const
    {
        return m_pTable->m_Sequence.Read ( m_pRecord );
    }

    const char* _ParamMetaRecord::GetName () const
    {
        return m_pTable->m_Name.Read ( m_pRecord );
    }

}
