#pragma once

namespace dotnetpe
{

    class ClassLayoutMetaTable;

    class _ClassLayoutMetaRecord : public MetaRecordBase < ClassLayoutMetaTable >
    {
    public:
                                    _ClassLayoutMetaRecord          () {}
                                    _ClassLayoutMetaRecord          ( ClassLayoutMetaTable* pTable, void* pRecord );

        word                        GetPackingSize                  () const;
        dword                       GetClassSize                    () const;
        dword                       GetParent                       () const;
    };

    class ClassLayoutMetaTable : public MetaTableBase < ClassLayoutMetaTable, _ClassLayoutMetaRecord >
    {
        friend _ClassLayoutMetaRecord;

    public:
                                    ClassLayoutMetaTable            ();
        virtual                     ~ClassLayoutMetaTable           ();

        enum { TYPE = METATABLE_CLASSLAYOUT };
        virtual eType               GetType                         () const    { return (eType)TYPE; }

        virtual void                Init                            ( TableLayer* pTableLayer );

    private:
        WordMetaFieldReader         m_PackingSize;
        DwordMetaFieldReader        m_ClassSize;
        TypeDefIndexMetaFieldReader m_Parent;
    };

}
