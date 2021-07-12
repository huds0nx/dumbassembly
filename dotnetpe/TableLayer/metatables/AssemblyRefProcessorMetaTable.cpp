#include "StdInc.h"

namespace dotnetpe
{

    AssemblyRefProcessorMetaTable::AssemblyRefProcessorMetaTable ()
    {

    }

    AssemblyRefProcessorMetaTable::~AssemblyRefProcessorMetaTable ()
    {

    }

    void AssemblyRefProcessorMetaTable::Init ( TableLayer* pTableLayer )
    {
        MetaTableBase < AssemblyRefProcessorMetaTable, _AssemblyRefProcessorMetaRecord >::Init ( pTableLayer );

        m_iRecordSize += m_Processor.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_AssemblyRef.Init ( pTableLayer, m_iRecordSize );
    }

    _AssemblyRefProcessorMetaRecord::_AssemblyRefProcessorMetaRecord ( AssemblyRefProcessorMetaTable* pTable, void* pRecord )
        : MetaRecordBase < AssemblyRefProcessorMetaTable > ( pTable, pRecord )
    {

    }

    dword _AssemblyRefProcessorMetaRecord::GetProcessor () const
    {
        return m_pTable->m_Processor.Read ( m_pRecord );
    }

    dword _AssemblyRefProcessorMetaRecord::GetAssemblyRef () const
    {
        return m_pTable->m_AssemblyRef.Read ( m_pRecord );
    }

}
