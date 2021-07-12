#pragma once

namespace dotnetpe
{

    class FieldMetaTable;

    class _FieldMetaRecord : public MetaRecordBase < FieldMetaTable >
    {
    public:
                                    _FieldMetaRecord            () {}
                                    _FieldMetaRecord            ( FieldMetaTable* pTable, void* pRecord );

        word                        GetFlags                    () const;
        const char*                 GetName                     () const;
        const void*                 GetSignature                () const;
    };

    class FieldMetaTable : public MetaTableBase < FieldMetaTable, _FieldMetaRecord >
    {
        friend _FieldMetaRecord;

    public:
                                    FieldMetaTable              ();
        virtual                     ~FieldMetaTable             ();

        enum { TYPE = METATABLE_FIELD };
        virtual eType               GetType                     () const    { return (eType)TYPE; }

        virtual void                Init                        ( TableLayer* pTableLayer );

    private:
        WordMetaFieldReader         m_Flags;
        StringsIndexMetaFieldReader m_Name;
        BlobIndexMetaFieldReader    m_Signature;
    };

}
