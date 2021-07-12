#pragma once

namespace dotnetpe
{

    class FieldSig : public Signature
    {
    public:
                                FieldSig                ( StructureLayer* pStructureLayer, const void* pSig );
                                ~FieldSig               () {}

        TypeSigElem             GetTypeSigElem          ()  { return m_TypeSigElem; }

    private:
        TypeSigElem             m_TypeSigElem;
    };

}
