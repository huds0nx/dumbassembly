#include "StdInc.h"

namespace dotnetpe
{

    StreamLayer::StreamLayer ( DotNetPE* pPE )
        : m_pPE ( pPE )
    {
        m_pTableStream = NULL;
        m_pStringsStream = NULL;
        m_pUSStream = NULL;
        m_pGUIDStream = NULL;
        m_pBlobStream = NULL;
    }

    StreamLayer::~StreamLayer ()
    {

    }

    void StreamLayer::Init ( void* pMetaHeader, dword dwMetaHeaderSize )
    {
        MemoryStream metaHeader ( pMetaHeader, dwMetaHeaderSize, true );
        dword dwSignature = metaHeader.ReadUInt32 ();
        assert ( dwSignature == COR20_METAHEADER_SIGNATURE );
        word wMajorVersion = metaHeader.ReadUInt16 ();
        word wMinorVersion = metaHeader.ReadUInt16 ();
        dword dwReserved = metaHeader.ReadUInt32 ();
        dword dwVersionLength = metaHeader.ReadUInt32 ();
        if ( dwVersionLength & 3 )
            dwVersionLength = (dwVersionLength & ~3) + 4;
        metaHeader.Seek ( dwVersionLength );
        word wFlags = metaHeader.ReadUInt16 ();
        dword dwNumStreams = metaHeader.ReadUInt16 ();

        m_MetaStreams.reserve ( dwNumStreams );
        for ( dword i = 0; i < dwNumStreams; i++ )
        {
            dword dwOffset = metaHeader.ReadUInt32 ();
            dword dwSize = metaHeader.ReadUInt32 ();
            const char* pszName = (const char *)metaHeader.Data () + metaHeader.GetPosition ();
            while ( metaHeader.ReadChar () != '\0' );
            while ( metaHeader.GetPosition () & 3 )
                metaHeader.ReadChar ();
            m_MetaStreams.push_back ( MetaStream ( pszName, (byte *)pMetaHeader + dwOffset, dwSize ) );
        }

        m_pTableStream   = GetMetaStream ( "#~" );
        m_pStringsStream = GetMetaStream ( "#Strings" );
        m_pUSStream      = GetMetaStream ( "#US" );
        m_pGUIDStream    = GetMetaStream ( "#GUID" );
        m_pBlobStream    = GetMetaStream ( "#Blob" );
    }

    int StreamLayer::NumMetaStreams () const
    {
        return m_MetaStreams.size ();
    }

    MetaStream* StreamLayer::GetMetaStream ( int iIndex )
    {
        if ( iIndex < 0 || iIndex >= m_MetaStreams.size () )
            return NULL;

        return &m_MetaStreams [ iIndex ];
    }

    MetaStream* StreamLayer::GetMetaStream ( const char* pszName )
    {
        MetaStreams_t::iterator it;
        for ( it = m_MetaStreams.begin (); it != m_MetaStreams.end (); it++ )
        {
            if ( strcmp ( (*it).Name (), pszName ) == 0 )
                return &(*it);
        }
        return NULL;
    }

}
