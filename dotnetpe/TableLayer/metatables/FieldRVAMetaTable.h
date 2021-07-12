#pragma once

namespace dotnetpe
{

    class FieldRVAMetaTable;

    class _FieldRVAMetaRecord : public MetaRecordBase < FieldRVAMetaTable >
    {
    public:
                                _FieldRVAMetaRecord             () {}
                                _FieldRVAMetaRecord             ( FieldRVAMetaTable* pTable, void* pRecord );

        dword                   GetRVA                          () const;
        dword                   GetField                        () const;
    };

    class FieldRVAMetaTable : public MetaTableBase < FieldRVAMetaTable, _FieldRVAMetaRecord >
    {
        friend _FieldRVAMetaRecord;

    public:
                                FieldRVAMetaTable               ();
        virtual                 ~FieldRVAMetaTable              ();

        enum { TYPE = METATABLE_FIELDRVA };
        virtual eType           GetType                         () const    { return (eType)TYPE; }

        virtual void            Init                            ( TableLayer* pTableLayer );

    private:
        DwordMetaFieldReader        m_RVA;
        FieldIndexMetaFieldReader   m_Field;
    };

}
