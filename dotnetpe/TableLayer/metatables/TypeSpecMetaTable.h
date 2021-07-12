#pragma once

namespace dotnetpe
{

    class TypeSpecMetaTable;

    class _TypeSpecMetaRecord : public MetaRecordBase < TypeSpecMetaTable >
    {
    public:
                            _TypeSpecMetaRecord             () {}
                            _TypeSpecMetaRecord             ( TypeSpecMetaTable* pTable, void* pRecord );

        const void*         GetSignature                    () const;
    };

    class TypeSpecMetaTable : public MetaTableBase < TypeSpecMetaTable, _TypeSpecMetaRecord >
    {
        friend _TypeSpecMetaRecord;

    public:
                            TypeSpecMetaTable               ();
        virtual             ~TypeSpecMetaTable              ();

        enum { TYPE = METATABLE_TYPESPEC };
        virtual eType       GetType                         () const    { return (eType)TYPE; }

        virtual void        Init                            ( TableLayer* pTableLayer );

    private:
        BlobIndexMetaFieldReader    m_Signature;
    };

}
