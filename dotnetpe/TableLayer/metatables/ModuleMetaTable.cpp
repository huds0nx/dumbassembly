#include "StdInc.h"

namespace dotnetpe
{

    ModuleMetaTable::ModuleMetaTable ()
    {

    }

    ModuleMetaTable::~ModuleMetaTable ()
    {

    }

    void ModuleMetaTable::Init ( TableLayer* pTableLayer )
    {
        MetaTableBase < ModuleMetaTable, _ModuleMetaRecord >::Init ( pTableLayer );

        m_iRecordSize += m_Generation.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Name.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Mvid.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_EncId.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_EncBaseId.Init ( pTableLayer, m_iRecordSize );
    }

    _ModuleMetaRecord::_ModuleMetaRecord ( ModuleMetaTable* pTable, void* pRecord)
        : MetaRecordBase < ModuleMetaTable > ( pTable, pRecord )
    {

    }

    word _ModuleMetaRecord::GetGeneration () const
    {
        return m_pTable->m_Generation.Read ( m_pRecord );
    }

    const char* _ModuleMetaRecord::GetName () const
    {
        return m_pTable->m_Name.Read ( m_pRecord );
    }

    const GUID* _ModuleMetaRecord::GetMvid () const
    {
        return m_pTable->m_Mvid.Read ( m_pRecord );
    }

    const GUID* _ModuleMetaRecord::GetEncId () const
    {
        return m_pTable->m_EncId.Read ( m_pRecord );
    }

    const GUID* _ModuleMetaRecord::GetEncBaseId () const
    {
        return m_pTable->m_EncBaseId.Read ( m_pRecord );
    }

}