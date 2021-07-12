#pragma once

namespace dotnetpe { class StreamLayer; }

#include "StreamLayer/MetaStream.h"

namespace dotnetpe
{

    class DotNetPE;

    class StreamLayer
    {
    public:
                            StreamLayer                 ( DotNetPE* pPE );
                            ~StreamLayer                ();

        void                Init                        ( void* pMetaHeader, dword dwMetaHeaderSize );

        int                 NumMetaStreams              () const;
        MetaStream*         GetMetaStream               ( int iIndex );
        MetaStream*         GetMetaStream               ( const char* pszName );

        MetaStream*         GetTableMetaStream          () const    { return m_pTableStream; }
        MetaStream*         GetStringsMetaStream        () const    { return m_pStringsStream; }
        MetaStream*         GetUSMetaStream             () const    { return m_pUSStream; }
        MetaStream*         GetGUIDMetaStream           () const    { return m_pGUIDStream; }
        MetaStream*         GetBlobMetaStream           () const    { return m_pBlobStream; }

    private:
        DotNetPE*           m_pPE;

        typedef std::vector < MetaStream > MetaStreams_t;
        MetaStreams_t       m_MetaStreams;

        MetaStream*         m_pTableStream;
        MetaStream*         m_pStringsStream;
        MetaStream*         m_pUSStream;
        MetaStream*         m_pGUIDStream;
        MetaStream*         m_pBlobStream;
    };

}
