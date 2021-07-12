#pragma once

namespace dotnetpe
{

    class TypeRef : public StructureItem < TypeRefMetaTable >, public Type
    {
    public:
                                TypeRef             ();
        virtual                 ~TypeRef            ();

        virtual bool            operator==          ( const Type& other ) const
        {
            return other.IsTypeRef () && Token () == static_cast < const TypeRef* > ( &other )->Token ();
        }

        virtual bool            IsTypeDef           () const    { return false; }
        virtual bool            IsTypeRef           () const    { return true; }

        virtual const char*     GetName             () const    { return m_Record.GetName (); }
        virtual const char*     GetNamespace        () const    { return m_Record.GetNamespace (); }

        dword                   NumMethods          ();
        MethodRef*              GetMethod           ( dword dwIndex );
        MethodRef*              GetMethod           ( const char* pszName );

    private:
        void                    FetchMethods        ();

        typedef prevector < MethodRef* > Methods_t;
        mutable Methods_t       m_Methods;
    };

}
