#pragma once

namespace dotnetpe
{

    class Resource : public StructureItem < ManifestResourceMetaTable >
    {
    public:
                                Resource                ();
        virtual                 ~Resource               ();

        const char*             GetName                 () const;
        void*                   Data                    () const;
        dword                   Size                    () const;
    };

}
