#pragma once

namespace dotnetpe
{

    class FieldMarshalMetaTable;

    class _FieldMarshalMetaRecord : public MetaRecordBase < FieldMarshalMetaTable >
    {
    public:
                                    _FieldMarshalMetaRecord         () {}
                                    _FieldMarshalMetaRecord         ( FieldMarshalMetaTable* pTable, void* pRecord );

        dword                       GetParent                       ( MetaTable*& pTable ) const;
        const void*                 GetNativeType                   () const;
    };

    class FieldMarshalMetaTable : public MetaTableBase < FieldMarshalMetaTable, _FieldMarshalMetaRecord >
    {
        friend _FieldMarshalMetaRecord;

    public:
                                    FieldMarshalMetaTable           ();
        virtual                     ~FieldMarshalMetaTable          ();

        enum { TYPE = METATABLE_FIELDMARSHAL };
        virtual eType               GetType                         () const    { return (eType)TYPE; }

        virtual void                Init                            ( TableLayer* pTableLayer );

    private:
        HasFieldMarshallIndexMetaFieldReader    m_Parent;
        BlobIndexMetaFieldReader                m_NativeType;
    };

}
