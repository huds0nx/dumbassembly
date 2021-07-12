#include "StdInc.h"

namespace dotnetpe
{

    MetaTable::MetaTable ()
        : m_pTableLayer ( NULL ), m_pRecords ( NULL ), m_iNumRecords ( 0 ), m_iRecordSize ( 0 )
    {

    }

    MetaTable::~MetaTable ()
    {

    }

    void MetaTable::Init ( TableLayer* pTableLayer )
    {
        m_pTableLayer = pTableLayer;
    }

    void MetaTable::SetRecordsBase ( void* pRecords )
    {
        m_pRecords = (byte *)pRecords;
    }

    int MetaTable::GetRecordSize () const
    {
        return m_iRecordSize;
    }

}
