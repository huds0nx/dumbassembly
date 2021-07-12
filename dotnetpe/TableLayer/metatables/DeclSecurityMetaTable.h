#pragma once

namespace dotnetpe
{

    class DeclSecurityMetaTable;

    class _DeclSecurityMetaRecord : public MetaRecordBase < DeclSecurityMetaTable >
    {
    public:
                                _DeclSecurityMetaRecord             () {}
                                _DeclSecurityMetaRecord             ( DeclSecurityMetaTable* pTable, void* pRecord );

        word                    GetAction                           () const;
        dword                   GetParent                           ( MetaTable*& pTable ) const;
        const void*             GetPermissionSet                    () const;
    };

    class DeclSecurityMetaTable : public MetaTableBase < DeclSecurityMetaTable, _DeclSecurityMetaRecord >
    {
        friend _DeclSecurityMetaRecord;

    public:
                                DeclSecurityMetaTable               ();
        virtual                 ~DeclSecurityMetaTable              ();

        enum { TYPE = METATABLE_DECLSECURITY };
        virtual eType           GetType                             () const    { return (eType)TYPE; }

        virtual void            Init                                ( TableLayer* pTableLayer );

    private:
        WordMetaFieldReader                 m_Action;
        HasDeclSecurityIndexMetaFieldReader m_Parent;
        BlobIndexMetaFieldReader            m_PermissionSet;
    };

}
