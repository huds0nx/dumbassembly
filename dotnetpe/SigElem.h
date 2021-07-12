#pragma once

namespace dotnetpe
{

    class SigElem
    {
        friend class SigElemStream;

    public:
                            SigElem                 ( StructureLayer* pStructureLayer );
                            ~SigElem                ();

    protected:
                            SigElem                 ( StructureLayer* pStructureLayer, const void* pData );

        StructureLayer*     m_pStructureLayer;
        const byte*         m_pData;
    };

}
