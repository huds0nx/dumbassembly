#include "StdInc.h"

namespace dotnetpe
{

    DotNetPE::DotNetPE ( const wchar_t* pwszFileName )
        : pe::PE ( pwszFileName ),
          m_StreamLayer ( this ),
          m_TableLayer ( this ),
          m_StructureLayer ( this )
    {
        Init ();
    }

    DotNetPE::DotNetPE ( byte* pData, dword dwSize )
        : pe::PE ( pData, dwSize ),
          m_StreamLayer ( this ),
          m_TableLayer ( this ),
          m_StructureLayer ( this )
    {
        Init ();
    }

    DotNetPE::~DotNetPE ()
    {

    }

    void DotNetPE::Init ()
    {
        if ( !Data () )
            return;

        IMAGE_COR20_HEADER* pCor20Header = GetCor20Header ();
        assert ( pCor20Header );

        void* pMetaHeader = RVAToPtr ( pCor20Header->MetaData.VirtualAddress );
        dword dwMetaHeaderSize = pCor20Header->MetaData.Size;
        assert ( pMetaHeader );

        m_StreamLayer.Init ( pMetaHeader, dwMetaHeaderSize );
        m_TableLayer.Init ();
        m_StructureLayer.Init ();
    }

    IMAGE_COR20_HEADER* DotNetPE::GetCor20Header () const
    {
        return reinterpret_cast < IMAGE_COR20_HEADER* > (
            GetDirectoryData ( IMAGE_DIRECTORY_ENTRY_COMHEADER ) );
    }

    std::wstring DotNetPE::GetUserString ( dword dwOffset ) const
    {
        MetaStream* pUsStream = GetUSMetaStream ();
        assert ( dwOffset < pUsStream->Size () );

        MemoryStream strStream ( (byte *)pUsStream->Data () + dwOffset, pUsStream->Size () - dwOffset, true );
        dword dwSize = strStream.ReadVarUInt32 ();
        assert ( strStream.GetPosition () + dwSize <= strStream.Size () );
        return strStream.ReadUtf16String ( (dwSize - 1) / sizeof(wchar_t) );
    }

}
