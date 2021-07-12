#pragma once

namespace dotnetpe
{

    class Field : public StructureItem < FieldMetaTable >
    {
    public:
                                Field               () {}
        virtual                 ~Field              () {}

        const char*             GetName             () const    { return m_Record.GetName (); }

        CorElementType          GetTypeKind         () const;
        Type*                   GetClass            () const;
        void*                   GetData             () const;

        enum eVisibility
        {
            PRIVATE,
            PROTECTED,
            INTERNAL,
            PUBLIC
        };
        eVisibility             GetVisibility       () const;

        bool                    IsStatic            () const;
    };

}
