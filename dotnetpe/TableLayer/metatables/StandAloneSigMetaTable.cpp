#include "StdInc.h"

namespace dotnetpe
{

    StandAloneSigMetaTable::StandAloneSigMetaTable ()
    {

    }

    StandAloneSigMetaTable::~StandAloneSigMetaTable ()
    {

    }

    void StandAloneSigMetaTable::Init ( TableLayer* pTableLayer )
    {
        MetaTableBase < StandAloneSigMetaTable, _StandAloneSigMetaRecord >::Init ( pTableLayer );

        m_iRecordSize += m_Signature.Init ( pTableLayer, m_iRecordSize );
    }

    _StandAloneSigMetaRecord::_StandAloneSigMetaRecord ( StandAloneSigMetaTable* pTable, void* pRecord )
        : MetaRecordBase < StandAloneSigMetaTable > ( pTable, pRecord )
    {

    }

    const void* _StandAloneSigMetaRecord::GetSignature () const
    {
        return m_pTable->m_Signature.Read ( m_pRecord );
    }

}
