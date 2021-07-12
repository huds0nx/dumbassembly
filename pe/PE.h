#pragma once

namespace pe
{

    class PE
    {
    public:
                                PE                      ( const wchar_t* pwszFileName );
                                PE                      ( byte* pData, dword dwSize );
                                ~PE                     ();

        const wchar_t*          GetFileName             () const    { return m_wstrFileName.c_str (); }

        void                    Close                   ();

        bool                    IsBad                   () const    { return Data () == NULL; }

        byte*                   Data                    () const    { return m_pData; }
        dword                   RawSize                 () const    { return m_dwSize; }
        dword                   VirtualSize             () const;

        dword                   ImageBase               () const;

        void                    FixNumberOfDirectories  () const;

        void*                   GetDirectoryData        ( int iIndex ) const;
        dword                   GetDirectorySize        ( int iIndex ) const;

        int                     NumSections             () const;
        Section                 GetSection              ( int iIndex ) const;

        void*                   RVAToPtr                ( dword dwRVA ) const;
        dword                   PtrToRVA                ( void* ptr ) const;

        dword                   RawToRVA                ( dword dwRaw ) const;
        dword                   RVAToRaw                ( dword dwRVA ) const;

    private:
        void                    Init                    ();

    protected:
        std::wstring            m_wstrFileName;

        MappedFile              m_MappedFile;
        byte*                   m_pData;
        dword                   m_dwSize;

        bool                    m_bX64;
        IMAGE_NT_HEADERS32*     m_pNtHeaders32;
        IMAGE_NT_HEADERS64*     m_pNtHeaders64;
        IMAGE_SECTION_HEADER*   m_pSections;
    };

}
