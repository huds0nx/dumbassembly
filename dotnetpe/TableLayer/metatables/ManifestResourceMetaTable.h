#pragma once

namespace dotnetpe
{

    class ManifestResourceMetaTable;

    class _ManifestResourceMetaRecord : public MetaRecordBase < ManifestResourceMetaTable >
    {
    public:
                            _ManifestResourceMetaRecord     () {}
                            _ManifestResourceMetaRecord     ( ManifestResourceMetaTable* pTable, void* pRecord );

        dword               GetOffset                       () const;
        dword               GetFlags                        () const;
        const char*         GetName                         () const;
        dword               GetImplementation               ( MetaTable*& pTable ) const;
    };

    class ManifestResourceMetaTable : public MetaTableBase < ManifestResourceMetaTable, _ManifestResourceMetaRecord >
    {
        friend _ManifestResourceMetaRecord;

    public:
                            ManifestResourceMetaTable       ();
        virtual             ~ManifestResourceMetaTable      ();

        enum { TYPE = METATABLE_MANIFESTRESOURCE };
        virtual eType       GetType                         () const    { return (eType)TYPE; }

        virtual void        Init                            ( TableLayer* pTableLayer );

    private:
        DwordMetaFieldReader                m_Offset;
        DwordMetaFieldReader                m_Flags;
        StringsIndexMetaFieldReader         m_Name;
        ImplementationIndexMetaFieldReader  m_Implementation;
    };

}
