#pragma once

namespace dotnetpe
{

    class NestedClassMetaTable;

    class _NestedClassMetaRecord : public MetaRecordBase < NestedClassMetaTable >
    {
    public:
                        _NestedClassMetaRecord          () {}
                        _NestedClassMetaRecord          ( NestedClassMetaTable* pTable, void* pRecord );

        dword           GetNestedClass                  () const;
        dword           GetEnclosingClass               () const;
    };

    class NestedClassMetaTable : public MetaTableBase < NestedClassMetaTable, _NestedClassMetaRecord >
    {
        friend _NestedClassMetaRecord;

    public:
                            NestedClassMetaTable            ();
        virtual             ~NestedClassMetaTable           ();

        enum { TYPE = METATABLE_NESTEDCLASS };
        virtual eType       GetType                         () const    { return (eType)TYPE; }

        virtual void        Init                            ( TableLayer* pTableLayer );

    private:
        TypeDefIndexMetaFieldReader m_NestedClass;
        TypeDefIndexMetaFieldReader m_EnclosingClass;
    };

}
