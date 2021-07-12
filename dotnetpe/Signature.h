#pragma once

namespace dotnetpe
{

    class Signature
    {
    public:
                                Signature               ( StructureLayer* pStructureLayer, const void* pSig );
                                ~Signature              () {}

    protected:
        SigElemStream           GetStream               ();

        StructureLayer*         m_pStructureLayer;
        dword                   m_dwLength;
        const byte*             m_pSig;
    };

}
