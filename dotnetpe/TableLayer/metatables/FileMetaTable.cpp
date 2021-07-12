#include "StdInc.h"

namespace dotnetpe
{

    FileMetaTable::FileMetaTable ()
    {

    }

    FileMetaTable::~FileMetaTable ()
    {

    }

    void FileMetaTable::Init ( TableLayer* pTableLayer )
    {
        MetaTableBase < FileMetaTable, _FileMetaRecord >::Init ( pTableLayer );

        m_iRecordSize += m_Flags.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Name.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_HashValue.Init ( pTableLayer, m_iRecordSize );
    }

    _FileMetaRecord::_FileMetaRecord ( FileMetaTable* pTable, void* pRecord )
        : MetaRecordBase < FileMetaTable > ( pTable, pRecord )
    {

    }

    dword _FileMetaRecord::GetFlags () const
    {
        return m_pTable->m_Flags.Read ( m_pRecord );
    }

    const char* _FileMetaRecord::GetName () const
    {
        return m_pTable->m_Name.Read ( m_pRecord );
    }

    const void* _FileMetaRecord::GetHashValue () const
    {
        return m_pTable->m_HashValue.Read ( m_pRecord );
    }

}
