#pragma once

namespace dotnetpe
{

    class ArrayShapeSigElem : public SigElem
    {
        friend class SigElemStream;

    public:
                                ArrayShapeSigElem           ( StructureLayer* pStructureLayer );
                                ~ArrayShapeSigElem          ();

        void                    Read                        ( SigElemStream* pStream );

    protected:
                                ArrayShapeSigElem           ( StructureLayer* pStructureLayer, const void* pData );
    };

}
