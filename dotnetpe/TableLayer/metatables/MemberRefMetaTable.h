#pragma once

namespace dotnetpe
{

    class MemberRefMetaTable;

    class _MemberRefMetaRecord : public MetaRecordBase < MemberRefMetaTable >
    {
    public:
                                _MemberRefMetaRecord        () {}
                                _MemberRefMetaRecord        ( MemberRefMetaTable* pTable, void* pRecord );

        dword                   GetClass                    ( MetaTable*& pTable ) const;
        const char*             GetName                     () const;
        const void*             GetSignature                () const;
    };

    class MemberRefMetaTable : public MetaTableBase < MemberRefMetaTable, _MemberRefMetaRecord >
    {
        friend _MemberRefMetaRecord;

    public:
                                MemberRefMetaTable          ();
        virtual                 ~MemberRefMetaTable         ();

        enum { TYPE = METATABLE_MEMBERREF };
        virtual eType           GetType                     () const    { return (eType)TYPE; }

        virtual void            Init                        ( TableLayer* pTableLayer );

    private:
        MemberRefParentIndexMetaFieldReader m_Class;
        StringsIndexMetaFieldReader         m_Name;
        BlobIndexMetaFieldReader            m_Signature;
    };

}
