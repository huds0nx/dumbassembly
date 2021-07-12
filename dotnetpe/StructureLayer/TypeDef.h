#pragma once

namespace dotnetpe
{

    class TypeDef : public StructureItem < TypeDefMetaTable >, public Type
    {
    public:
                                TypeDef             ();
        virtual                 ~TypeDef            ();

        virtual void            Init                ( StructureLayer* pStructureLayer, dword dwIndex );

        virtual bool            operator==          ( const Type& other ) const
        {
            return other.IsTypeDef () && Token () == static_cast < const TypeDef* > ( &other )->Token ();
        }

        virtual bool            IsTypeDef           () const    { return true; }
        virtual bool            IsTypeRef           () const    { return false; }

        virtual const char*     GetName             () const    { return m_Record.GetName (); }
        virtual const char*     GetNamespace        () const    { return m_Record.GetNamespace (); }

        int                     GetSize             () const;
        int                     GetPack             () const;

        dword                   NumFields           () const    { return m_dwFieldsEndIndex - m_dwFieldsStartIndex; }
        Field*                  GetField            ( dword dwIndex ) const;
        Field*                  GetField            ( const char* pszName ) const;

        dword                   NumMethods          () const    { return m_dwMethodsEndIndex - m_dwMethodsStartIndex; }
        MethodDef*              GetMethod           ( dword dwIndex ) const;
        MethodDef*              GetMethod           ( const char* pszName ) const;

        Type*                   GetBaseType         () const;

    private:
        void                    FetchClassLayout    () const;

        mutable int             m_iSize;
        mutable int             m_iPack;

        dword                   m_dwFieldsStartIndex;
        dword                   m_dwFieldsEndIndex;

        dword                   m_dwMethodsStartIndex;
        dword                   m_dwMethodsEndIndex;
    };

}
