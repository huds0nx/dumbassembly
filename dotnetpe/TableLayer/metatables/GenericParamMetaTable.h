#pragma once

namespace dotnetpe
{

    class GenericParamMetaTable;

    class _GenericParamMetaRecord : public MetaRecordBase < GenericParamMetaTable >
    {
    public:
                            _GenericParamMetaRecord         () {}
                            _GenericParamMetaRecord         ( GenericParamMetaTable* pTable, void* pRecord );

        word                GetNumber                       () const;
        word                GetFlags                        () const;
        dword               GetOwner                        ( MetaTable*& pTable ) const;
        const char*         GetName                         () const;
    };

    class GenericParamMetaTable : public MetaTableBase < GenericParamMetaTable, _GenericParamMetaRecord >
    {
        friend _GenericParamMetaRecord;

    public:
                            GenericParamMetaTable           ();
        virtual             ~GenericParamMetaTable          ();

        enum { TYPE = METATABLE_GENERICPARAM };
        virtual eType       GetType                         () const    { return (eType)TYPE; }

        virtual void        Init                            ( TableLayer* pTableLayer );

    private:
        WordMetaFieldReader                 m_Number;
        WordMetaFieldReader                 m_Flags;
        TypeOrMethodDefIndexMetaFieldReader m_Owner;
        StringsIndexMetaFieldReader         m_Name;
    };

}
