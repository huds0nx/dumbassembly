#pragma once

namespace dotnetpe
{

    class CustomAttributeMetaTable;

    class _CustomAttributeMetaRecord : public MetaRecordBase < CustomAttributeMetaTable >
    {
    public:
                                _CustomAttributeMetaRecord      () {}
                                _CustomAttributeMetaRecord      ( CustomAttributeMetaTable* pTable, void* pRecord );

        dword                   GetParent                       ( MetaTable*& pTable ) const;
        dword                   GetType                         ( MetaTable*& pTable ) const;
        const void*             GetValue                        () const;
    };

    class CustomAttributeMetaTable : public MetaTableBase < CustomAttributeMetaTable, _CustomAttributeMetaRecord >
    {
        friend _CustomAttributeMetaRecord;

    public:
                                CustomAttributeMetaTable        ();
        virtual                 ~CustomAttributeMetaTable       ();

        enum { TYPE = METATABLE_CUSTOMATTRIBUTE };
        virtual eType           GetType                         () const    { return (eType)TYPE; }

        virtual void            Init                            ( TableLayer* pTableLayer );

    private:
        HasCustomAttributeIndexMetaFieldReader  m_Parent;
        CustomAttributeTypeIndexMetaFieldReader m_Type;
        BlobIndexMetaFieldReader                m_Value;
    };

}
