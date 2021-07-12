#include "StdInc.h"

namespace dotnetpe
{

    EventMetaTable::EventMetaTable ()
    {

    }

    EventMetaTable::~EventMetaTable ()
    {

    }

    void EventMetaTable::Init ( TableLayer* pTableLayer )
    {
        MetaTableBase < EventMetaTable, _EventMetaRecord >::Init ( pTableLayer );

        m_iRecordSize += m_EventFlags.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Name.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_EventType.Init ( pTableLayer, m_iRecordSize );
    }

    _EventMetaRecord::_EventMetaRecord ( EventMetaTable* pTable, void* pRecord )
        : MetaRecordBase < EventMetaTable > ( pTable, pRecord )
    {

    }

    word _EventMetaRecord::GetEventFlags () const
    {
        return m_pTable->m_EventFlags.Read ( m_pRecord );
    }

    const char* _EventMetaRecord::GetName () const
    {
        return m_pTable->m_Name.Read ( m_pRecord );
    }

    dword _EventMetaRecord::GetEventType ( MetaTable*& pTable ) const
    {
        return m_pTable->m_EventType.Read ( m_pRecord, pTable );
    }

}
