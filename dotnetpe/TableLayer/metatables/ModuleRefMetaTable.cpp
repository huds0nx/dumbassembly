#include "StdInc.h"

namespace dotnetpe
{

    ModuleRefMetaTable::ModuleRefMetaTable ()
    {

    }

    ModuleRefMetaTable::~ModuleRefMetaTable ()
    {

    }

    void ModuleRefMetaTable::Init ( TableLayer* pTableLayer )
    {
        MetaTableBase < ModuleRefMetaTable, _ModuleRefMetaRecord >::Init ( pTableLayer );

        m_iRecordSize += m_Name.Init ( pTableLayer, m_iRecordSize );
    }

    _ModuleRefMetaRecord::_ModuleRefMetaRecord ( ModuleRefMetaTable* pTable, void* pRecord )
        : MetaRecordBase < ModuleRefMetaTable > ( pTable, pRecord )
    {

    }

    const char* _ModuleRefMetaRecord::GetName () const
    {
        return m_pTable->m_Name.Read ( m_pRecord );
    }

}
