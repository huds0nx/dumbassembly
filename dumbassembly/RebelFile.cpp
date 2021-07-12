#include "StdInc.h"

RebelFile::RebelFile ()
{

}

RebelFile::~RebelFile ()
{

}


void RebelFile::ReplaceStream ( const char* pszName, const void* pData, dword dwSize )
{
    StreamInfo stream;
    stream.m_strName = pszName;
    stream.m_pData = pData;
    stream.m_dwSize = dwSize;

    m_StreamInfos.push_back ( stream );
}

void RebelFile::ReplaceMethod ( dword dwToken, const void* pCode, dword dwCodeSize, const void* pEH, dword dwEHSize )
{
    MethodInfo method;
    method.m_dwToken = dwToken;
    method.m_pCode = pCode;
    method.m_dwCodeSize = dwCodeSize;
    method.m_pEH = pEH;
    method.m_dwEHSize = dwEHSize;

    m_MethodInfos.push_back ( method );
}

bool RebelFile::Write ( const wchar_t* pwszFile ) const
{
    FILE* pFile = _wfopen ( pwszFile, L"wb+" );
    if ( !pFile )
        return false;

    // Fill in the header
    REBEL_NET_BASE header;
    memset ( &header, 0, sizeof(header) );
    header.Signature = REBEL_NET_SIGNATURE;

    int iOffset = sizeof(header);

    header.NumberOfStreams = m_StreamInfos.size ();
    header.StreamsOffset = iOffset;
    for ( StreamInfos_t::const_iterator it = m_StreamInfos.begin (); it != m_StreamInfos.end (); it++ )
    {
        iOffset += it->m_strName.size () + 1 + 4 + it->m_dwSize;
    }

    header.NumberOfMethods = m_MethodInfos.size ();
    header.MethodsOffset = iOffset;

    // Write header
    fwrite ( &header, sizeof(header), 1, pFile );

    // Write streams
    for ( StreamInfos_t::const_iterator it = m_StreamInfos.begin (); it != m_StreamInfos.end (); it++ )
    {
        fwrite ( it->m_strName.c_str (), 1, it->m_strName.size () + 1, pFile );
        fwrite ( &it->m_dwSize, 4, 1, pFile );
        fwrite ( it->m_pData, 1, it->m_dwSize, pFile );
    }

    // Write methods
    for ( MethodInfos_t::const_iterator it = m_MethodInfos.begin (); it != m_MethodInfos.end (); it++ )
    {
        REBEL_METHOD methodHeader;
        memset ( &methodHeader, 0, sizeof(methodHeader) );

        methodHeader.Token = it->m_dwToken;
        methodHeader.CodeSize = it->m_dwCodeSize;
        methodHeader.ExtraSectionsSize = it->m_dwEHSize;
        fwrite ( &methodHeader, sizeof(methodHeader), 1, pFile );

        fwrite ( it->m_pCode, 1, it->m_dwCodeSize, pFile );
        fwrite ( it->m_pEH, 1, it->m_dwEHSize, pFile );
    }

    fclose ( pFile );
    return true;
}

bool RebelFile::Run ( const wchar_t* pwszFileToRebuild ) const
{
    return Run ( pwszFileToRebuild, pwszFileToRebuild );
}

bool RebelFile::Run ( const wchar_t* pwszFileToRebuild, const wchar_t* pwszOutFile ) const
{
    if ( Empty () )
        return true;

    wchar_t wszRebelFile[MAX_PATH];
    if ( !GetTempFileName ( L".", L"", 0, wszRebelFile ) )
        return false;

    if ( !Write ( wszRebelFile ) )
        return false;

    std::wstring wstrCmd;
    wstrCmd += L"RebelDotNET.exe /ra /asm=\"";
    wstrCmd += pwszFileToRebuild;
    wstrCmd += L"\" /reb=\"";
    wstrCmd += wszRebelFile;
    wstrCmd += L"\" /out=\"";
    wstrCmd += pwszOutFile;
    wstrCmd += L"\"";

    STARTUPINFO startInfo;
    PROCESS_INFORMATION procInfo;
    memset ( &startInfo, 0, sizeof(startInfo) );
    startInfo.cb = sizeof(startInfo);
    if ( !CreateProcess ( NULL, const_cast < wchar_t* > ( wstrCmd.c_str () ), NULL, NULL,
        false, 0, NULL, NULL, &startInfo, &procInfo ) )
    {
        DeleteFile ( wszRebelFile );
        return false;
    }

    WaitForSingleObject ( procInfo.hThread, INFINITE );
    CloseHandle ( procInfo.hThread );
    CloseHandle ( procInfo.hProcess );

    DeleteFile ( wszRebelFile );
    return true;
}