#pragma once

namespace dotnetpe
{

    class SigElemStream : public MemoryStream
    {
    public:
                                    SigElemStream               ( StructureLayer* pStructureLayer, const void* pData, int iSize, bool bReadOnly = false );
                                    SigElemStream               ( const SigElem* pElem, bool bSkipElemTypeByte );
        virtual                     ~SigElemStream              ();

        bool                        CanReadCustomMod            () const;
        bool                        CanReadType                 () const;

        CustomModSigElem            ReadCustomMod               ();
        TypeDefOrRefEncodedSigElem  ReadTypeDefOrRefEncoded     ();
        ParamSigElem                ReadParam                   ();
        RetTypeSigElem              ReadRetType                 ();
        TypeSigElem                 ReadType                    ();
        ArrayShapeSigElem           ReadArrayShape              ();

    private:
        template < class T >
        T                           ReadElem                    ()
        {
            T result ( m_pStructureLayer, (byte *)Data () + GetPosition () );
            result.Read ( this );
            return result;
        }

        StructureLayer*             m_pStructureLayer;
    };

}
