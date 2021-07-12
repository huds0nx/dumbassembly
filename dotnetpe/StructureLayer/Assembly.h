#pragma once

namespace dotnetpe
{

    class Assembly : public StructureItem < AssemblyMetaTable >
    {
    public:
                            Assembly                () {}
        virtual             ~Assembly               () {}

        const char*         GetName                 () const    { return m_Record.GetName (); }

        const void*         GetPublicKey            () const;
        bool                GetPublicKeyToken       ( byte* pToken ) const;

		const void*			GetCustomAttributeData	( const char* pszAttributeTypeName ) const;
    };

}
