#pragma once

namespace dotnetpe
{

    class TypeSigElem : public SigElem
    {
        friend class SigElemStream;

    public:
                                    TypeSigElem             ( StructureLayer* pStructureLayer );
                                    ~TypeSigElem            ();

        void                        Read                    ( SigElemStream* pStream );

        CorElementType              GetElemType             () const;

        TypeSigElem                 GetArrayType            () const;
        ArrayShapeSigElem           GetArrayShape           () const;
        Type*                       GetClass                () const;
        Type*                       GetValueType            () const;

    protected:
                                    TypeSigElem             ( StructureLayer* pStructureLayer, const void* pData );
    };

}
