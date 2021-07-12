#pragma once

namespace dotnetpe
{

    class FieldLayoutMetaTable;

    class _FieldLayoutMetaRecord : public MetaRecordBase < FieldLayoutMetaTable >
    {
    public:
                                _FieldLayoutMetaRecord      () {}
                                _FieldLayoutMetaRecord      ( FieldLayoutMetaTable* pTable, void* pRecord );

        dword                   GetOffset                   () const;
        dword                   GetField                    () const;
    };

    class FieldLayoutMetaTable : public MetaTableBase < FieldLayoutMetaTable, _FieldLayoutMetaRecord >
    {
        friend _FieldLayoutMetaRecord;

    public:
                                FieldLayoutMetaTable        ();
        virtual                 ~FieldLayoutMetaTable       ();

        enum { TYPE = METATABLE_FIELDLAYOUT };
        virtual eType           GetType                     () const    { return (eType)TYPE; }

        virtual void            Init                        ( TableLayer* pTableLayer );

    private:
        DwordMetaFieldReader        m_Offset;
        FieldIndexMetaFieldReader   m_Field;
    };

}
