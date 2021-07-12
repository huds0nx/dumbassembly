#pragma once

namespace dotnetpe
{

    class InterfaceImplMetaTable;

    class _InterfaceImplMetaRecord : public MetaRecordBase < InterfaceImplMetaTable >
    {
    public:
                                _InterfaceImplMetaRecord    () {}
                                _InterfaceImplMetaRecord    ( InterfaceImplMetaTable* pTable, void* pRecord );

        dword                   GetClass                    () const;
        dword                   GetInterface                ( MetaTable*& pTable ) const;
    };

    class InterfaceImplMetaTable : public MetaTableBase < InterfaceImplMetaTable, _InterfaceImplMetaRecord >
    {
        friend _InterfaceImplMetaRecord;

    public:
                                InterfaceImplMetaTable      ();
        virtual                 ~InterfaceImplMetaTable     ();

        enum { TYPE = METATABLE_INTERFACEIMPL };
        virtual eType           GetType                     () const    { return (eType)TYPE; }

        virtual void            Init                        ( TableLayer* pTableLayer );

    private:
        TypeDefIndexMetaFieldReader         m_Class;
        TypeDefOrRefIndexMetaFieldReader    m_Interface;
    };

}
