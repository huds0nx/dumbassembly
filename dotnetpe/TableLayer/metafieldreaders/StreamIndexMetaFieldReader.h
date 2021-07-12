#pragma once

namespace dotnetpe
{

    template < class Derived, typename T >
    class StreamIndexMetaFieldReader : public MetaFieldReader
    {
    public:
        int                     Init            ( TableLayer* pTableLayer, MetaStream* pStream,
                                                  byte ucHeapOffsetSizesMask, int iFieldOffset )
        {
            MetaFieldReader::Init ( pTableLayer, iFieldOffset );
            m_pStream = pStream;
            m_bFat = ( pTableLayer->GetHeapOffsetSizes () & ucHeapOffsetSizesMask ) != 0;
            return m_bFat ? 4 : 2;
        }

        const T*                Read            ( void* pRecord ) const
        {
            dword dwOffset;
            if ( m_bFat )
                dwOffset = *GetFieldPtr < dword > ( pRecord );
            else
                dwOffset = *GetFieldPtr < word > ( pRecord );

            if ( dwOffset == 0 )
                return NULL;

            dwOffset = static_cast < const Derived* > ( this )->AdjustOffset ( dwOffset );
            return (T *)( (byte *)m_pStream->Data () + dwOffset );
        }

    protected:
        dword                   AdjustOffset    ( dword dwOffset ) const
        {
            return dwOffset;
        }

        MetaStream*             m_pStream;
        bool                    m_bFat;
    };

    // ------------------------------------

    class StringsIndexMetaFieldReader : public StreamIndexMetaFieldReader < StringsIndexMetaFieldReader, char >
    {
    public:
        int                     Init            ( TableLayer* pTableLayer, int iFieldOffset );
    };

    // ------------------------------------

    class GUIDIndexMetaFieldReader : public StreamIndexMetaFieldReader < GUIDIndexMetaFieldReader, GUID >
    {
    public:
        int                     Init            ( TableLayer* pTableLayer, int iFieldOffset );

    protected:
        friend StreamIndexMetaFieldReader < GUIDIndexMetaFieldReader, GUID >;

        dword                   AdjustOffset    ( dword dwOffset ) const
        {
            return dwOffset - 1;
        }
    };

    // ------------------------------------

    class BlobIndexMetaFieldReader : public StreamIndexMetaFieldReader < BlobIndexMetaFieldReader, void >
    {
    public:
        int                     Init            ( TableLayer* pTableLayer, int iFieldOffset );
    };

}
