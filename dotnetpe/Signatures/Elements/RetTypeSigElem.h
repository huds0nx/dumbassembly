#pragma once

namespace dotnetpe
{

    class RetTypeSigElem : public SigElem
    {
        friend class SigElemStream;

    public:
                                RetTypeSigElem              ( StructureLayer* pStructureLayer );
                                ~RetTypeSigElem             ();

        void                    Read                        ( SigElemStream* pStream );

    protected:
                                RetTypeSigElem              ( StructureLayer* pStructureLayer, const void* pData );
    };

}
