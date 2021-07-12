#pragma once

namespace dotnetpe
{

    template < typename T >
    class SimpleMetaFieldReader : public MetaFieldReader
    {
    public:
        int                 Init            ( TableLayer* pTableLayer, int iFieldOffset )
        {
            MetaFieldReader::Init ( pTableLayer, iFieldOffset );
            return sizeof(T);
        }

        T                   Read            ( void* pRecord ) const
        {
            return *GetFieldPtr < T > ( pRecord );
        }
    };


    typedef SimpleMetaFieldReader < byte >  ByteMetaFieldReader;
    typedef SimpleMetaFieldReader < word >  WordMetaFieldReader;
    typedef SimpleMetaFieldReader < dword > DwordMetaFieldReader;

}
