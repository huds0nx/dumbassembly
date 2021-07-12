#pragma once

namespace dotnetpe
{

    class MethodSemanticsMetaTable;

    class _MethodSemanticsMetaRecord : public MetaRecordBase < MethodSemanticsMetaTable >
    {
    public:
                                _MethodSemanticsMetaRecord          () {}
                                _MethodSemanticsMetaRecord          ( MethodSemanticsMetaTable* pTable, void* pRecord );

        word                    GetSemantics                        () const;
        dword                   GetMethod                           () const;
        dword                   GetAssociation                      ( MetaTable*& pTable ) const;
    };

    class MethodSemanticsMetaTable : public MetaTableBase < MethodSemanticsMetaTable, _MethodSemanticsMetaRecord >
    {
        friend _MethodSemanticsMetaRecord;

    public:
                                MethodSemanticsMetaTable            ();
        virtual                 ~MethodSemanticsMetaTable           ();

        enum { TYPE = METATABLE_METHODSEMANTICS };
        virtual eType           GetType                             () const    { return (eType)TYPE; }

        virtual void            Init                                ( TableLayer* pTableLayer );

    private:
        WordMetaFieldReader                 m_Semantics;
        MethodDefIndexMetaFieldReader       m_Method;
        HasSemanticsIndexMetaFieldReader    m_Association;
    };

}
