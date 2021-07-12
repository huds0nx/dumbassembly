#include "StdInc.h"

namespace dotnetpe
{

    PropertyMapMetaTable::PropertyMapMetaTable ()
    {

    }

    PropertyMapMetaTable::~PropertyMapMetaTable ()
    {

    }

    void PropertyMapMetaTable::Init ( TableLayer* pTableLayer )
    {
        MetaTableBase < PropertyMapMetaTable, _PropertyMapMetaRecord >::Init ( pTableLayer );

        m_iRecordSize += m_Parent.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_PropertyList.Init ( pTableLayer, m_iRecordSize );
    }

    _PropertyMapMetaRecord::_PropertyMapMetaRecord ( PropertyMapMetaTable* pTable, void* pRecord )
        : MetaRecordBase < PropertyMapMetaTable > ( pTable, pRecord )
    {

    }

    dword _PropertyMapMetaRecord::GetParent () const
    {
        return m_pTable->m_Parent.Read ( m_pRecord );
    }

    dword _PropertyMapMetaRecord::GetPropertyList () const
    {
        return m_pTable->m_PropertyList.Read ( m_pRecord );
    }

}
