#pragma once

namespace dotnetpe
{

    class MethodImplMetaTable;

    class _MethodImplMetaRecord : MetaRecordBase < MethodImplMetaTable >
    {
    public:
                                _MethodImplMetaRecord           () {}
                                _MethodImplMetaRecord           ( MethodImplMetaTable* pTable, void* pRecord );

        dword                   GetClass                        () const;
        dword                   GetMethodBody                   ( MetaTable*& pTable ) const;
        dword                   GetMethodDeclaration            ( MetaTable*& pTable ) const;
    };

    class MethodImplMetaTable : public MetaTableBase < MethodImplMetaTable, _MethodImplMetaRecord >
    {
        friend _MethodImplMetaRecord;

    public:
                                MethodImplMetaTable             ();
        virtual                 ~MethodImplMetaTable            ();

        enum { TYPE = METATABLE_METHODIMPL };
        virtual eType           GetType                         () const    { return (eType)TYPE; }

        virtual void            Init                            ( TableLayer* pTableLayer );

    private:
        TypeDefIndexMetaFieldReader         m_Class;
        MethodDefOrRefIndexMetaFieldReader  m_MethodBody;
        MethodDefOrRefIndexMetaFieldReader  m_MethodDeclaration;
    };

}
