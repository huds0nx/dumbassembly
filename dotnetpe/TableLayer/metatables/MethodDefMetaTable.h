#pragma once

namespace dotnetpe
{

    class MethodDefMetaTable;

    class _MethodDefMetaRecord : public MetaRecordBase < MethodDefMetaTable >
    {
    public:
                                    _MethodDefMetaRecord        () {}
                                    _MethodDefMetaRecord        ( MethodDefMetaTable* pTable, void* pRecord );

        dword                       GetRVA                      () const;
        word                        GetImplFlags                () const;
        word                        GetFlags                    () const;
        const char*                 GetName                     () const;
        const void*                 GetSignature                () const;
        dword                       GetParamList                () const;
    };

    class MethodDefMetaTable : public MetaTableBase < MethodDefMetaTable, _MethodDefMetaRecord >
    {
        friend _MethodDefMetaRecord;

    public:
                                    MethodDefMetaTable          ();
        virtual                     ~MethodDefMetaTable         ();

        enum { TYPE = METATABLE_METHODDEF };
        virtual eType               GetType                     () const    { return (eType)TYPE; }

        virtual void                Init                        ( TableLayer* pTableLayer );

    private:
        DwordMetaFieldReader        m_RVA;
        WordMetaFieldReader         m_ImplFlags;
        WordMetaFieldReader         m_Flags;
        StringsIndexMetaFieldReader m_Name;
        BlobIndexMetaFieldReader    m_Signature;
        ParamIndexMetaFieldReader   m_ParamList;
    };

}
