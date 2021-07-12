#include "StdInc.h"

namespace pe
{

    MappedFile::MappedFile ()
    {
        m_hFile = INVALID_HANDLE_VALUE;
        m_hMapping = NULL;
        m_pMapped = NULL;
    }

    MappedFile::MappedFile ( const wchar_t* pwszFileName )
    {
        m_hFile = INVALID_HANDLE_VALUE;
        m_hMapping = NULL;
        m_pMapped = NULL;

        Open ( pwszFileName );
    }

    MappedFile::~MappedFile ()
    {
        Close ();
    }

    bool MappedFile::Open ( const wchar_t* pwszFileName )
    {
        Close ();

        m_hFile = CreateFile ( pwszFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL,
            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
        if ( m_hFile == INVALID_HANDLE_VALUE )
            return false;

        m_hMapping = CreateFileMapping ( m_hFile, NULL, PAGE_READWRITE, 0, 0, NULL );
        if ( !m_hMapping )
        {
            CloseHandle ( m_hFile );
            m_hFile = INVALID_HANDLE_VALUE;
            return false;
        }

        m_pMapped = (byte *)MapViewOfFile ( m_hMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0 );
        if ( !m_pMapped )
        {
            CloseHandle ( m_hMapping );
            m_hMapping = NULL;
            CloseHandle ( m_hFile );
            m_hFile = INVALID_HANDLE_VALUE;
            return false;
        }
        return true;
    }

    void MappedFile::Close ()
    {
        if ( m_hFile != INVALID_HANDLE_VALUE )
        {
            if ( m_hMapping != NULL )
            {
                if ( m_pMapped )
                {
                    UnmapViewOfFile ( m_pMapped );
                    m_pMapped = NULL;
                }
                CloseHandle ( m_hMapping );
                m_hMapping = NULL;
            }
            CloseHandle ( m_hFile );
            m_hFile = INVALID_HANDLE_VALUE;
        }
    }

    byte* MappedFile::Data () const
    {
        return m_pMapped;
    }

    dword MappedFile::Size () const
    {
        if ( m_hFile == INVALID_HANDLE_VALUE )
            return 0;
        else
            return GetFileSize ( m_hFile, NULL );
    }

}
