#pragma once

namespace dotnetpe
{

    class TypeDefOrRefEncodedSigElem : public SigElem
    {
        friend class SigElemStream;

    public:
                                TypeDefOrRefEncodedSigElem  ( StructureLayer* pStructureLayer );
                                ~TypeDefOrRefEncodedSigElem ();

        void                    Read                        ( SigElemStream* pStream );

        Type*                   GetType                     () const;

    protected:
                                TypeDefOrRefEncodedSigElem  ( StructureLayer* pStructureLayer, const void* pData );

        typedef struct
        {
            dword dwTag : 2;
            dword dwIndex : 30;
        } TypeDefOrRefEncoded;
    };

}
