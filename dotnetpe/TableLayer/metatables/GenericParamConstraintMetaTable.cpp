#include "StdInc.h"

namespace dotnetpe
{

    GenericParamConstraintMetaTable::GenericParamConstraintMetaTable ()
    {

    }

    GenericParamConstraintMetaTable::~GenericParamConstraintMetaTable ()
    {

    }

    void GenericParamConstraintMetaTable::Init ( TableLayer* pTableLayer )
    {
        MetaTableBase < GenericParamConstraintMetaTable, _GenericParamConstraintMetaRecord >::Init ( pTableLayer );

        m_iRecordSize += m_Owner.Init ( pTableLayer, m_iRecordSize );
        m_iRecordSize += m_Constraint.Init ( pTableLayer, m_iRecordSize );
    }

    _GenericParamConstraintMetaRecord::_GenericParamConstraintMetaRecord ( GenericParamConstraintMetaTable* pTable, void* pRecord )
        : MetaRecordBase < GenericParamConstraintMetaTable > ( pTable, pRecord )
    {

    }

    dword _GenericParamConstraintMetaRecord::GetOwner () const
    {
        return m_pTable->m_Owner.Read ( m_pRecord );
    }

    dword _GenericParamConstraintMetaRecord::GetConstraint ( MetaTable*& pTable ) const
    {
        return m_pTable->m_Constraint.Read ( m_pRecord, pTable );
    }

}
