#pragma once

namespace pe
{

    class Section
    {
        friend class PE;

    public:
        void*                       Data                    () const    { return (byte *)m_pFile + RawOffset (); }

        dword                       VirtualOffset           () const    { return m_pHeader->VirtualAddress; }
        dword                       VirtualSize             () const    { return m_pHeader->Misc.VirtualSize; }
        dword                       RawOffset               () const    { return m_pHeader->PointerToRawData; }
        dword                       RawSize                 () const    { return m_pHeader->SizeOfRawData; }
        dword                       Characteristics         () const    { return m_pHeader->Characteristics; }

    private:
                                    Section                 ( void* pFile, IMAGE_SECTION_HEADER* pHeader );

        void*                       m_pFile;
        IMAGE_SECTION_HEADER*       m_pHeader;
    };

}
