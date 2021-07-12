#include "StdInc.h"

namespace dotnetpe
{

    InterfaceImplMetaTable::InterfaceImplMetaTable ()
    {

    }

    InterfaceImplMetaTable::~InterfaceImplMetaTable ()
    {

    }

    void InterfaceImplMetaTable::Init ( TableLayer* pTableLayer )
    {
        MetaTableBase < InterfaceImplMetaTable, _InterfaceImplMetaRecord >::Init ( pTableLayer );

        m_iRecordSize += m_Class.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Interface.Init ( pTableLayer, m_iRecordSize );
    }

    _InterfaceImplMetaRecord::_InterfaceImplMetaRecord ( InterfaceImplMetaTable* pTable, void* pRecord )
        : MetaRecordBase < InterfaceImplMetaTable > ( pTable, pRecord )
    {
        
    }

    dword _InterfaceImplMetaRecord::GetClass () const
    {
        return m_pTable->m_Class.Read ( m_pRecord );
    }

    dword _InterfaceImplMetaRecord::GetInterface ( MetaTable*& pTable ) const
    {
        return m_pTable->m_Interface.Read ( m_pRecord, pTable );
    }

}
