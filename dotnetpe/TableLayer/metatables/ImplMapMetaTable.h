#pragma once

namespace dotnetpe
{

    class ImplMapMetaTable;

    class _ImplMapMetaRecord : public MetaRecordBase < ImplMapMetaTable >
    {
    public:
                            _ImplMapMetaRecord              () {}
                            _ImplMapMetaRecord              ( ImplMapMetaTable* pTable, void* pRecord );

        word                GetMappingFlags                 () const;
        dword               GetMemberForwarded              ( MetaTable*& pTable ) const;
        const char*         GetImportName                   () const;
        dword               GetImportScope                  () const;
    };

    class ImplMapMetaTable : public MetaTableBase < ImplMapMetaTable, _ImplMapMetaRecord >
    {
        friend _ImplMapMetaRecord;

    public:
                            ImplMapMetaTable                ();
        virtual             ~ImplMapMetaTable               ();

        enum { TYPE = METATABLE_IMPLMAP };
        virtual eType       GetType                         () const    { return (eType)TYPE; }

        virtual void        Init                            ( TableLayer* pTableLayer );

    private:
        WordMetaFieldReader                 m_MappingFlags;
        MemberForwardedIndexMetaFieldReader m_MemberForwarded;
        StringsIndexMetaFieldReader         m_ImportName;
        ModuleRefIndexMetaFieldReader       m_ImportScope;
    };

}
