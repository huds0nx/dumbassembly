#pragma once

namespace dotnetpe
{

    class DotNetPE;

    class MetaFieldReader
    {
    public:
                        MetaFieldReader     () : m_iFieldOffset ( 0 ) {}

        void            Init                ( TableLayer* pTableLayer, int iFieldOffset )
        {
            m_pTableLayer = pTableLayer;
            m_iFieldOffset = iFieldOffset;
        }
        
    protected:
        template < typename T >
        T*              GetFieldPtr         ( void* pRecord ) const
        {
            return (T *)( (byte *)pRecord + m_iFieldOffset );
        }

        TableLayer*     m_pTableLayer;
        int             m_iFieldOffset;
    };

}
