#include "StdInc.h"

byte* Util::GetFileContent ( const wchar_t* pwszFileName, dword* pdwFileSize )
{
    FILE* pFile = _wfopen ( pwszFileName, L"rb" );
    if ( !pFile )
        return NULL;

    fseek ( pFile, 0, SEEK_END );
    dword dwFileSize = ftell ( pFile );
    rewind ( pFile );
    byte* pMem = new byte[dwFileSize];
    if ( !pMem )
    {
        fclose ( pFile );
        return NULL;
    }

    fread ( pMem, 1, dwFileSize, pFile );
    fclose ( pFile );
    if ( pdwFileSize )
        *pdwFileSize = dwFileSize;

    return pMem;
}

bool Util::membytecmp ( void* pMem, byte ucValue, int iSize )
{
    byte* pTest = (byte *)pMem;
    for ( int i = 0; i < iSize; i++ )
    {
        if ( pTest[i] != ucValue )
            return false;
    }
    return true;
}

std::wstring Util::StringToWString ( const std::string& str )
{
    std::wstring wstr;
    wstr.resize ( MultiByteToWideChar ( CP_UTF8, 0, str.c_str (), str.size (), NULL, 0 ) );
    MultiByteToWideChar ( CP_UTF8, 0, str.c_str (), str.size (), const_cast < wchar_t* > ( wstr.c_str () ), wstr.size () );
    return wstr;
}

std::string Util::WStringToString ( const std::wstring& wstr )
{
    std::string str;
    str.resize ( WideCharToMultiByte ( CP_UTF8, 0, wstr.c_str (), wstr.size (), NULL, 0, NULL, false ) );
    WideCharToMultiByte ( CP_UTF8, 0, wstr.c_str (), wstr.size (), const_cast < char* > ( str.c_str () ), str.size (), NULL, false );
    return str;
}

std::string Util::GuidToString ( const GUID* pGuid )
{
    return WStringToString ( GuidToWString ( pGuid ) );
}

std::wstring Util::GuidToWString ( const GUID* pGuid )
{
    std::wstring wstrGuid;
    wstrGuid.resize ( 38 );
    wstrGuid.reserve ( 39 );
    StringFromGUID2 ( *pGuid, const_cast < wchar_t* > ( wstrGuid.data () ), wstrGuid.capacity () );
    std::transform ( wstrGuid.begin (), wstrGuid.end (), wstrGuid.begin (), [](wchar_t c) { return tolower(c); } );
    return wstrGuid;
}
