#pragma once

namespace dotnetpe
{

    class MethodRef : public StructureItem < MemberRefMetaTable >
    {
    public:
                            MethodRef               ();
        virtual             ~MethodRef              ();

        const char*         GetName                 () const    { return m_Record.GetName (); }
        class TypeRef*      GetType                 () const;
    };

}
