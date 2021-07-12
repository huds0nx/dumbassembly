#include "StdInc.h"

namespace dotnetpe
{

    EventMapMetaTable::EventMapMetaTable ()
    {

    }

    EventMapMetaTable::~EventMapMetaTable ()
    {

    }

    void EventMapMetaTable::Init ( TableLayer* pTableLayer )
    {
        MetaTableBase < EventMapMetaTable, _EventMapMetaRecord >::Init ( pTableLayer );

        m_iRecordSize += m_Parent.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_EventList.Init ( pTableLayer, m_iRecordSize );
    }

    _EventMapMetaRecord::_EventMapMetaRecord ( EventMapMetaTable* pTable, void* pRecord )
        : MetaRecordBase < EventMapMetaTable > ( pTable, pRecord )
    {

    }

    dword _EventMapMetaRecord::GetParent () const
    {
        return m_pTable->m_Parent.Read ( m_pRecord );
    }

    dword _EventMapMetaRecord::GetEventList () const
    {
        return m_pTable->m_EventList.Read ( m_pRecord );
    }

}
