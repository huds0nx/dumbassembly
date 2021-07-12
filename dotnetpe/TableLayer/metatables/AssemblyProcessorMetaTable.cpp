#include "StdInc.h"

namespace dotnetpe
{

    AssemblyProcessorMetaTable::AssemblyProcessorMetaTable ()
    {

    }

    AssemblyProcessorMetaTable::~AssemblyProcessorMetaTable ()
    {

    }

    void AssemblyProcessorMetaTable::Init ( TableLayer* pTableLayer )
    {
        MetaTableBase < AssemblyProcessorMetaTable, _AssemblyProcessorMetaRecord >::Init ( pTableLayer );

        m_iRecordSize += m_Processor.Init ( pTableLayer, m_iRecordSize );
    }

    _AssemblyProcessorMetaRecord::_AssemblyProcessorMetaRecord ( AssemblyProcessorMetaTable* pTable, void* pRecord )
        : MetaRecordBase < AssemblyProcessorMetaTable > ( pTable, pRecord )
    {

    }

    dword _AssemblyProcessorMetaRecord::GetProcessor () const
    {
        return m_pTable->m_Processor.Read ( m_pRecord );
    }

}
