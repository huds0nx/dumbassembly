#pragma once

namespace pe
{

    class MappedFile
    {
    public:
                                MappedFile              ();
                                MappedFile              ( const wchar_t* pwszFileName );
                                ~MappedFile             ();

        bool                    Open                    ( const wchar_t* pwszFileName );
        void                    Close                   ();

        byte*                   Data                    () const;
        dword                   Size                    () const;

    private:
        HANDLE                  m_hFile;
        HANDLE                  m_hMapping;
        byte*                   m_pMapped;
    };

}
