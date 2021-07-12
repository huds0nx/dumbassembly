#pragma once

class Util
{
public:
    static byte*            GetFileContent          ( const wchar_t* pwszFileName, dword* pdwFileSize = NULL );

    static bool             membytecmp              ( void* pMem, byte ucValue, int iSize );

    static std::wstring     StringToWString         ( const std::string& str );
    static std::string      WStringToString         ( const std::wstring& wstr );

    static std::string      GuidToString            ( const GUID* pGuid );
    static std::wstring     GuidToWString           ( const GUID* pGuid );
};
