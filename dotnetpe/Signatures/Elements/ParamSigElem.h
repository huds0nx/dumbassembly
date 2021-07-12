#pragma once

namespace dotnetpe
{

    class ParamSigElem : public SigElem
    {
        friend class SigElemStream;

    public:
                            ParamSigElem            ( StructureLayer* pStructureLayer );
                            ~ParamSigElem           ();

        void                Read                    ( SigElemStream* pStream );

    protected:
                            ParamSigElem            ( StructureLayer* pStructureLayer, const void* pData );
    };

}
