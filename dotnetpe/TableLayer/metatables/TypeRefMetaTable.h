#pragma once

namespace dotnetpe
{

    class TypeRefMetaTable;

    class _TypeRefMetaRecord : public MetaRecordBase < TypeRefMetaTable >
    {
    public:
                            _TypeRefMetaRecord  () {}
                            _TypeRefMetaRecord  ( TypeRefMetaTable* pTable, void* pRecord );

        dword               GetResolutionScope  ( MetaTable*& pTable ) const;
        const char*         GetName             () const;
        const char*         GetNamespace        () const;
    };

    class TypeRefMetaTable : public MetaTableBase < TypeRefMetaTable, _TypeRefMetaRecord >
    {
        friend _TypeRefMetaRecord;

    public:
                            TypeRefMetaTable    ();
        virtual             ~TypeRefMetaTable   ();

        enum { TYPE = METATABLE_TYPEREF };
        virtual eType       GetType             () const    { return (eType)TYPE; }

        virtual void        Init                ( TableLayer* pTableLayer );

    private:
        ResolutionScopeIndexMetaFieldReader m_ResolutionScope;
        StringsIndexMetaFieldReader         m_TypeName;
        StringsIndexMetaFieldReader         m_TypeNamespace;
    };

}
