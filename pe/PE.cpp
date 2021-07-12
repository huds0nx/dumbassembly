#include "StdInc.h"

namespace pe
{

    PE::PE ( const wchar_t* pwszFileName )
        : m_MappedFile ( pwszFileName )
    {
        m_wstrFileName = pwszFileName;
        m_pData = m_MappedFile.Data ();
        m_dwSize = m_MappedFile.Size ();

        Init ();
    }

    PE::PE ( byte* pData, dword dwSize )
    {
        m_pData = pData;
        m_dwSize = dwSize;

        Init ();
    }

    PE::~PE ()
    {

    }

    void PE::Init ()
    {
        m_pNtHeaders32 = NULL;
        m_pNtHeaders64 = NULL;
        m_pSections = NULL;

        if ( IsBad () )
            return;

        IMAGE_DOS_HEADER* pDosHeader = (IMAGE_DOS_HEADER *)Data ();
        if ( pDosHeader->e_magic != IMAGE_DOS_SIGNATURE )
        {
            Close ();
            return;
        }
        IMAGE_NT_HEADERS* pNtHeaders = (IMAGE_NT_HEADERS *)(Data () + pDosHeader->e_lfanew);
        if ( pNtHeaders->Signature != IMAGE_NT_SIGNATURE )
        {
            Close ();
            return;
        }

        if ( pNtHeaders->FileHeader.Machine == IMAGE_FILE_MACHINE_I386 )
        {
            m_bX64 = false;
            m_pNtHeaders32 = reinterpret_cast < IMAGE_NT_HEADERS32* > ( pNtHeaders );
            m_pSections = IMAGE_FIRST_SECTION ( m_pNtHeaders32 );
        }
        else if ( pNtHeaders->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64 )
        {
            m_bX64 = true;
            m_pNtHeaders64 = reinterpret_cast < IMAGE_NT_HEADERS64* > ( pNtHeaders );
            m_pSections = IMAGE_FIRST_SECTION ( m_pNtHeaders64 );
        }
        else
        {
            Close ();
        }
    }

    void PE::Close ()
    {
        m_MappedFile.Close ();
        m_pData = NULL;
        m_dwSize = 0;
        m_wstrFileName.clear ();
    }

    dword PE::VirtualSize () const
    {
        if ( !m_bX64 )
            return m_pNtHeaders32->OptionalHeader.SizeOfImage;
        else
            return m_pNtHeaders64->OptionalHeader.SizeOfImage;
    }

    void PE::FixNumberOfDirectories () const
    {
        if ( !m_bX64 )
            m_pNtHeaders32->OptionalHeader.NumberOfRvaAndSizes = IMAGE_NUMBEROF_DIRECTORY_ENTRIES;
        else
            m_pNtHeaders64->OptionalHeader.NumberOfRvaAndSizes = IMAGE_NUMBEROF_DIRECTORY_ENTRIES;
    }

    void* PE::GetDirectoryData ( int iIndex ) const
    {
        if ( iIndex < 0 || iIndex >= IMAGE_NUMBEROF_DIRECTORY_ENTRIES )
            return NULL;

        dword dwRVA = 0;
        if ( !m_bX64 )
            dwRVA = m_pNtHeaders32->OptionalHeader.DataDirectory [ iIndex ].VirtualAddress;
        else
            dwRVA = m_pNtHeaders64->OptionalHeader.DataDirectory [ iIndex ].VirtualAddress;
        return dwRVA ? RVAToPtr ( dwRVA ) : NULL;
    }

    dword PE::GetDirectorySize ( int iIndex ) const
    {
        if ( iIndex < 0 || iIndex >= IMAGE_NUMBEROF_DIRECTORY_ENTRIES )
            return 0;

        if ( !m_bX64 )
            return m_pNtHeaders32->OptionalHeader.DataDirectory [ iIndex ].Size;
        else
            return m_pNtHeaders64->OptionalHeader.DataDirectory [ iIndex ].Size;
    }

    int PE::NumSections () const
    {
        if ( !m_bX64 )
            return m_pNtHeaders32 ? m_pNtHeaders32->FileHeader.NumberOfSections : 0;
        else
            return m_pNtHeaders64 ? m_pNtHeaders64->FileHeader.NumberOfSections : 0;
    }

    Section PE::GetSection ( int iIndex ) const
    {
        assert ( iIndex >= 0 && iIndex < NumSections () );
        return Section ( Data (), &m_pSections [ iIndex ] );
    }

    void* PE::RVAToPtr ( dword dwRVA ) const
    {
        dword dwRaw = RVAToRaw ( dwRVA );
        if ( dwRaw == ~0 )
            return NULL;

        return Data () + dwRaw;
    }

    dword PE::PtrToRVA ( void* ptr ) const
    {
        if ( ptr < Data () || ptr >= Data () + RawSize () )
            return ~0;

        dword dwRaw = (byte *)ptr - Data ();
        return RawToRVA ( dwRaw );
    }

    dword PE::RawToRVA ( dword dwRaw ) const
    {
        if ( dwRaw >= RawSize () )
            return ~0;

        if ( dwRaw < GetSection ( 0 ).RawOffset () )
            return dwRaw;

        for ( int i = NumSections () - 1; i >= 0; i-- )
        {
            Section sect = GetSection ( i );
            if ( dwRaw >= sect.RawOffset () )
                return dwRaw - sect.RawOffset () + sect.VirtualOffset ();
        }
        return ~0;
    }

    dword PE::RVAToRaw ( dword dwRVA ) const
    {
        if ( dwRVA >= VirtualSize () )
            return ~0;

        if ( dwRVA < GetSection ( 0 ).RawOffset () )
            return dwRVA;

        for ( int i = 0; i < NumSections (); i++ )
        {
            Section sect = GetSection ( i );
            if ( dwRVA >= sect.VirtualOffset () && dwRVA < sect.VirtualOffset () + sect.VirtualSize () )
                return dwRVA - sect.VirtualOffset () + sect.RawOffset ();
        }
        return ~0;
    }

    dword PE::ImageBase () const
    {
        if ( !m_bX64 )
            return m_pNtHeaders32->OptionalHeader.ImageBase;
        else
            return m_pNtHeaders64->OptionalHeader.ImageBase;
    }

}
