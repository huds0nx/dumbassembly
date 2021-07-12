#pragma once

namespace dotnetpe
{

    class GenericParamConstraintMetaTable;

    class _GenericParamConstraintMetaRecord : public MetaRecordBase < GenericParamConstraintMetaTable >
    {
    public:
                            _GenericParamConstraintMetaRecord       () {}
                            _GenericParamConstraintMetaRecord       ( GenericParamConstraintMetaTable* pTable, void* pRecord );

        dword               GetOwner                                () const;
        dword               GetConstraint                           ( MetaTable*& pTable ) const;
    };

    class GenericParamConstraintMetaTable : public MetaTableBase < GenericParamConstraintMetaTable, _GenericParamConstraintMetaRecord >
    {
        friend _GenericParamConstraintMetaRecord;

    public:
                            GenericParamConstraintMetaTable         ();
        virtual             ~GenericParamConstraintMetaTable        ();

        enum { TYPE = METATABLE_GENERICPARAMCONSTRAINT };
        virtual eType       GetType                                 () const    { return (eType)TYPE; }

        virtual void        Init                                    ( TableLayer* pTableLayer );

    private:
        GenericParamIndexMetaFieldReader    m_Owner;
        TypeDefOrRefIndexMetaFieldReader    m_Constraint;
    };

}
