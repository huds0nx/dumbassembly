#include "StdInc.h"

namespace dotnetpe
{

    TypeSpecMetaTable::TypeSpecMetaTable ()
    {

    }

    TypeSpecMetaTable::~TypeSpecMetaTable ()
    {

    }

    void TypeSpecMetaTable::Init ( TableLayer* pTableLayer )
    {
        MetaTableBase < TypeSpecMetaTable, _TypeSpecMetaRecord >::Init ( pTableLayer );

        m_iRecordSize += m_Signature.Init ( pTableLayer, m_iRecordSize );
    }

    _TypeSpecMetaRecord::_TypeSpecMetaRecord ( TypeSpecMetaTable* pTable, void* pRecord )
        : MetaRecordBase < TypeSpecMetaTable > ( pTable, pRecord )
    {

    }

    const void* _TypeSpecMetaRecord::GetSignature () const
    {
        return m_pTable->m_Signature.Read ( m_pRecord );
    }

}
