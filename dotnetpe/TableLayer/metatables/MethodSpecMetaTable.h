#pragma once

namespace dotnetpe
{

    class MethodSpecMetaTable;

    class _MethodSpecMetaRecord : public MetaRecordBase < MethodSpecMetaTable >
    {
    public:
                                _MethodSpecMetaRecord       () {}
                                _MethodSpecMetaRecord       ( MethodSpecMetaTable* pTable, void* pRecord );

        dword                   GetMethod                   ( MetaTable*& pTable ) const;
        const void*             GetSignature                () const;
    };

    class MethodSpecMetaTable : public MetaTableBase < MethodSpecMetaTable, _MethodSpecMetaRecord >
    {
        friend _MethodSpecMetaRecord;

    public:
                                MethodSpecMetaTable         ();
        virtual                 ~MethodSpecMetaTable        ();

        enum { TYPE = METATABLE_METHODSPEC };
        virtual eType           GetType                     () const    { return (eType)TYPE; }

        virtual void            Init                        ( TableLayer* pTableLayer );

    private:
        MethodDefOrRefIndexMetaFieldReader  m_Method;
        BlobIndexMetaFieldReader            m_Signature;
    };

}
