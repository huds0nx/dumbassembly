#pragma once

namespace dotnetpe
{

    class CustomModSigElem : public SigElem
    {
        friend class SigElemStream;

    public:
                                CustomModSigElem        ( StructureLayer* pStructureLayer );
                                ~CustomModSigElem       ();

        void                    Read                    ( SigElemStream* pStream );

        bool                    IsRequired              () const;
        Type*                   GetType                 () const;

    protected:
                                CustomModSigElem        ( StructureLayer* pStructureLayer, const void* pData );
    };

}
