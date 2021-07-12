#pragma once

namespace dotnetpe
{

    class Module : public StructureItem < ModuleMetaTable >
    {
    public:
                                Module              () {}
        virtual                 ~Module             () {}

        const char*             GetName             () const    { return m_Record.GetName (); }

        const GUID*             GetMvid             () const    { return m_Record.GetMvid (); }

        dword                   NumTypes            () const;
        TypeDef*                GetType             ( dword dwIndex ) const;
        TypeDef*                GetType             ( const char* pszFullName ) const;

        dword                   NumMethods          () const;

        dword                   NumReferencedTypes  () const;
        TypeRef*                GetReferencedType   ( dword dwIndex ) const;
        TypeRef*                GetReferencedType   ( const char* pszFullName ) const;

        class FindInfo
        {
            friend Module;

        public:
                                    FindInfo            ();
                                    ~FindInfo           () {}

            TypeDef*                Type                () const;
            MethodDef*              Method              () const;
            byte*                   Instruction         () const;
            
        private:
            Module*                 m_pModule;
            dword                   m_dwTypeIndex;
            dword                   m_dwMethodIndex;
            byte*                   m_pInstr;
            byte*                   m_pMethodEnd;
        };

        bool                    FindNextInstr       ( FindInfo& info, std::tr1::function < bool (FindInfo&) > handler );
    };

}
