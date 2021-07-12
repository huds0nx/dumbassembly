#pragma once

namespace dotnetpe
{

    class TypeDefMetaTable;

    class _TypeDefMetaRecord : public MetaRecordBase < TypeDefMetaTable >
    {
    public:
                                        _TypeDefMetaRecord  () {}
                                        _TypeDefMetaRecord  ( TypeDefMetaTable* pTable, void* pRecord );

        CorTypeAttr                     GetFlags            () const;
        const char*                     GetName             () const;
        const char*                     GetNamespace        () const;
        dword                           GetExtends          ( MetaTable*& pTable ) const;
        dword                           GetFieldList        () const;
        dword                           GetMethodList       () const;
    };

    class TypeDefMetaTable : public MetaTableBase < TypeDefMetaTable, _TypeDefMetaRecord >
    {
        friend _TypeDefMetaRecord;

    public:
                                        TypeDefMetaTable    ();
        virtual                         ~TypeDefMetaTable   ();

        enum { TYPE = METATABLE_TYPEDEF };
        virtual eType                   GetType             () const    { return (eType)TYPE; }

        virtual void                    Init                ( TableLayer* pTableLayer );

    private:
        DwordMetaFieldReader                m_Flags;
        StringsIndexMetaFieldReader         m_TypeName;
        StringsIndexMetaFieldReader         m_TypeNamespace;
        TypeDefOrRefIndexMetaFieldReader    m_Extends;
        FieldIndexMetaFieldReader           m_FieldList;
        MethodDefIndexMetaFieldReader       m_MethodList;
    };

}
