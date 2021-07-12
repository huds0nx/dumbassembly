#include "StdInc.h"

namespace dotnetpe
{

    ExportedTypeMetaTable::ExportedTypeMetaTable ()
    {

    }

    ExportedTypeMetaTable::~ExportedTypeMetaTable ()
    {

    }

    void ExportedTypeMetaTable::Init ( TableLayer* pTableLayer )
    {
        MetaTableBase < ExportedTypeMetaTable, _ExportedTypeMetaRecord >::Init ( pTableLayer );

        m_iRecordSize += m_Flags.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_TypeDefId.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_TypeName.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_TypeNamespace.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Implementation.Init ( pTableLayer, m_iRecordSize );
    }

    _ExportedTypeMetaRecord::_ExportedTypeMetaRecord ( ExportedTypeMetaTable* pTable, void* pRecord )
        : MetaRecordBase < ExportedTypeMetaTable > ( pTable, pRecord )
    {

    }

    dword _ExportedTypeMetaRecord::GetFlags () const
    {
        return m_pTable->m_Flags.Read ( m_pRecord );
    }

    dword _ExportedTypeMetaRecord::GetTypeDefId () const
    {
        return m_pTable->m_TypeDefId.Read ( m_pRecord );
    }

    const char* _ExportedTypeMetaRecord::GetTypeName () const
    {
        return m_pTable->m_TypeName.Read ( m_pRecord );
    }

    const char* _ExportedTypeMetaRecord::GetTypeNamespace () const
    {
        return m_pTable->m_TypeNamespace.Read ( m_pRecord );
    }

    dword _ExportedTypeMetaRecord::GetImplementation ( MetaTable*& pTable ) const
    {
        return m_pTable->m_Implementation.Read ( m_pRecord, pTable );
    }

}
