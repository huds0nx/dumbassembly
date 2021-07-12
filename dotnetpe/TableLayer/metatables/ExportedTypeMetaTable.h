#pragma once

namespace dotnetpe
{

    class ExportedTypeMetaTable;

    class _ExportedTypeMetaRecord : public MetaRecordBase < ExportedTypeMetaTable >
    {
    public:
                            _ExportedTypeMetaRecord         () {}
                            _ExportedTypeMetaRecord         ( ExportedTypeMetaTable* pTable, void* pRecord );

        dword               GetFlags                        () const;
        dword               GetTypeDefId                    () const;
        const char*         GetTypeName                     () const;
        const char*         GetTypeNamespace                () const;
        dword               GetImplementation               ( MetaTable*& pTable ) const;
    };

    class ExportedTypeMetaTable : public MetaTableBase < ExportedTypeMetaTable, _ExportedTypeMetaRecord >
    {
        friend _ExportedTypeMetaRecord;

    public:
                            ExportedTypeMetaTable           ();
        virtual             ~ExportedTypeMetaTable          ();

        enum { TYPE = METATABLE_EXPORTEDTYPE };
        virtual eType       GetType                         () const    { return (eType)TYPE; }

        virtual void        Init                            ( TableLayer* pTableLayer );

    private:
        DwordMetaFieldReader                m_Flags;
        DwordMetaFieldReader                m_TypeDefId;
        StringsIndexMetaFieldReader         m_TypeName;
        StringsIndexMetaFieldReader         m_TypeNamespace;
        ImplementationIndexMetaFieldReader  m_Implementation;
    };

}
