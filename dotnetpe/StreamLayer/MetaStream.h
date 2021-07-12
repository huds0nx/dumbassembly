#pragma once

namespace dotnetpe
{

    class MetaStream
    {
    public:
                                MetaStream                  ( const char* pszName, void* pData, dword dwSize );

        const char*             Name                        () const    { return m_pszName; }
        void*                   Data                        () const    { return m_pData; }
        dword                   Size                        () const    { return m_dwSize; }

    protected:
        const char*             m_pszName;
        void*                   m_pData;
        dword                   m_dwSize;
    };

}
