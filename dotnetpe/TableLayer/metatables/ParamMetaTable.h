#pragma once

namespace dotnetpe
{

    class ParamMetaTable;

    class _ParamMetaRecord : public MetaRecordBase < ParamMetaTable >
    {
    public:
                                    _ParamMetaRecord                () {}
                                    _ParamMetaRecord                ( ParamMetaTable* pTable, void* pRecord );

        word                        GetFlags                        () const;
        word                        GetSequence                     () const;
        const char*                 GetName                         () const;
    };

    class ParamMetaTable : public MetaTableBase < ParamMetaTable, _ParamMetaRecord >
    {
        friend _ParamMetaRecord;

    public:
                                    ParamMetaTable                  ();
        virtual                     ~ParamMetaTable                 ();

        enum { TYPE = METATABLE_PARAM };
        virtual eType               GetType                         () const    { return (eType)TYPE; }

        virtual void                Init                            ( TableLayer* pTableLayer );

    private:
        WordMetaFieldReader         m_Flags;
        WordMetaFieldReader         m_Sequence;
        StringsIndexMetaFieldReader m_Name;
    };

}
