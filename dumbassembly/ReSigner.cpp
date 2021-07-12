#include "StdInc.h"

using namespace dotnetpe;

ReSigner::ReSigner ( const wchar_t* pwszKeyFilePath )
    : m_pKeys ( NULL ), m_dwKeysSize ( 0 )
{
    m_pKeys = Util::GetFileContent ( pwszKeyFilePath, &m_dwKeysSize );
    m_wstrKeyFilePath = pwszKeyFilePath;
}

ReSigner::~ReSigner()
{
    if ( m_pKeys )
        delete[] m_pKeys;
}

bool ReSigner::IsAssemblySigned ( const wchar_t* pwszAssemblyPath )
{
    DotNetPE pe ( pwszAssemblyPath );
    return pe.GetAssembly ()->GetPublicKey () != NULL;
}

bool ReSigner::UpdateAssembly ( const wchar_t* pwszPatchedAssemblyPath, const wchar_t* pwszOriginalAssemblyPath )
{
    if ( !m_pKeys )
        return false;

    DotNetPE pe ( pwszPatchedAssemblyPath );
    if ( pe.IsBad () )
        return false;

    // Change any public key tokens and arrays
    UpdatePublicKeyTokenStrings ( &pe, pwszOriginalAssemblyPath );
    UpdatePublicKeyArrays ( &pe, pwszOriginalAssemblyPath );
    pe.Close ();

    // Sign the patched assembly with the new key
    STARTUPINFO startInfo;
    memset ( &startInfo, 0, sizeof(startInfo) );
    startInfo.cb = sizeof(startInfo);
    PROCESS_INFORMATION procInfo;
    std::wstring wstrCmdLine = L"ReSign.exe \"";
    wstrCmdLine += pwszPatchedAssemblyPath;
    wstrCmdLine += L"\" \"";
    wstrCmdLine += m_wstrKeyFilePath;
    wstrCmdLine += L"\"";
    if ( !CreateProcess ( NULL, const_cast < wchar_t* > ( wstrCmdLine.c_str () ), NULL, NULL, false, 0, NULL, NULL, &startInfo, &procInfo ) )
        return false;

    WaitForSingleObject ( procInfo.hThread, INFINITE );
    CloseHandle ( procInfo.hThread );
    CloseHandle ( procInfo.hProcess );
    return true;
}

bool ReSigner::CreateStrongNameKeyFile ( const wchar_t* pwszKeyFilePath, int iKeySize )
{
    byte* pKeys;
    dword dwKeysSize;
    if ( !StrongNameKeyGenEx ( NULL, 0, iKeySize, &pKeys, &dwKeysSize ) )
        return false;

    FILE* pFile = _wfopen ( pwszKeyFilePath, L"wb+" );
    if ( !pFile )
    {
        StrongNameFreeBuffer ( pKeys );
        return false;
    }

    fwrite ( pKeys, 1, dwKeysSize, pFile );
    fclose ( pFile );
    StrongNameFreeBuffer ( pKeys );
    return true;
}

void ReSigner::UpdatePublicKeyTokenStrings ( DotNetPE* pPE, const wchar_t* pwszOriginalAssemblyPath )
{
    // Get the token of the replacement key
    std::wstring wstrOriginalToken = GetAssemblyPublicKeyToken ( pwszOriginalAssemblyPath );
    std::wstring wstrNewToken = GetKeyPairPublicKeyToken ( m_pKeys, m_dwKeysSize );
    if ( wstrOriginalToken.empty () || wstrNewToken.empty () )
        return;

    // Search for and patch the public key token in the #US stream
    ReplaceData (
        (byte *)pPE->GetUSMetaStream ()->Data (),
        pPE->GetUSMetaStream ()->Size (),
        (byte *)wstrOriginalToken.data (),
        wstrOriginalToken.size () * sizeof(wchar_t),
        (byte *)wstrNewToken.data ()
    );
    
    // Search for and patch the public key token in resources (UTF8 and UTF16)
    std::string strOriginalToken = Util::WStringToString ( wstrOriginalToken );
    std::string strNewToken = Util::WStringToString ( wstrNewToken );
    for ( int i = 0; i < pPE->NumResources (); i++ )
    {
        Resource* pResource = pPE->GetResource ( i );
        ReplaceData (
            (byte *)pResource->Data (),
            pResource->Size (),
            (byte *)wstrOriginalToken.data (),
            wstrOriginalToken.size () * sizeof(wchar_t),
            (byte *)wstrNewToken.data ()
        );
        ReplaceData (
            (byte *)pResource->Data (),
            pResource->Size (),
            (byte *)strOriginalToken.data (),
            strOriginalToken.size (),
            (byte *)strNewToken.data ()
        );
    }
}

void ReSigner::UpdatePublicKeyArrays ( dotnetpe::DotNetPE* pPE, const wchar_t* pwszOriginalAssemblyPath )
{
    dword dwOriginalPublicKeyLength;
    byte* pOriginalPublicKey = GetAssemblyPublicKey ( pwszOriginalAssemblyPath, &dwOriginalPublicKeyLength );
    if ( !pOriginalPublicKey )
        return;

    dword dwNewPublicKeyLength;
    byte* pNewPublicKey = GetKeyPairPublicKey ( m_pKeys, m_dwKeysSize, &dwNewPublicKeyLength );
    if ( !pNewPublicKey )
    {
        delete[] pOriginalPublicKey;
        return;
    }

    assert ( dwNewPublicKeyLength == dwOriginalPublicKeyLength );

    ReplaceData (
        pPE->Data (),
        pPE->RawSize (),
        pOriginalPublicKey,
        dwOriginalPublicKeyLength,
        pNewPublicKey
    );

    delete[] pNewPublicKey;
    delete[] pOriginalPublicKey;
}

byte* ReSigner::GetAssemblyPublicKey ( const wchar_t* pwszAssemblyPath, dword* pdwPublicKeySize )
{
    DotNetPE pe ( pwszAssemblyPath );
    if ( !pe.GetAssembly ()->GetPublicKey () )
    {
        pe.Close ();
        return NULL;
    }

    MemoryStream publicKeyStream ( pe.GetAssembly ()->GetPublicKey (), 0x7FFFFFFF, true );
    *pdwPublicKeySize = publicKeyStream.ReadVarUInt32 ();
    byte* pPublicKey = new byte[*pdwPublicKeySize];
    publicKeyStream.ReadBytes ( pPublicKey, *pdwPublicKeySize );
    pe.Close ();
    return pPublicKey;
}

dword ReSigner::GetAssemblyPublicKeyBitsSize ( const wchar_t* pwszAssemblyPath )
{
    dword dwPublicKeySize;
    byte* pPublicKey = GetAssemblyPublicKey ( pwszAssemblyPath, &dwPublicKeySize );
    if ( !pPublicKey )
        return 0;

    dword dwBitsSize = ((_RSAPUBKEY *)(pPublicKey + offsetof(PublicKeyBlob, PublicKey) + sizeof(BLOBHEADER)))->bitlen;
    delete[] pPublicKey;
    return dwBitsSize;
}

std::wstring ReSigner::GetAssemblyPublicKeyToken ( const wchar_t* pwszAssemblyPath )
{
    byte* pToken;
    dword dwTokenLength;
    if ( !StrongNameTokenFromAssembly ( pwszAssemblyPath, &pToken, &dwTokenLength ) )
        return std::wstring ();

    std::wstring wstrToken = BytesToHex ( pToken, dwTokenLength );
    StrongNameFreeBuffer ( pToken );
    return wstrToken;
}

byte* ReSigner::GetKeyPairPublicKey ( byte* pKeyPair, dword dwKeyPairSize, dword* pdwPublicKeySize )
{
    byte* pPublicKey;
    if ( !StrongNameGetPublicKey ( NULL, pKeyPair, dwKeyPairSize, &pPublicKey, pdwPublicKeySize ) )
        return NULL;

    byte* pPublicKeyCopy = new byte[*pdwPublicKeySize];
    memcpy ( pPublicKeyCopy, pPublicKey, *pdwPublicKeySize );
    StrongNameFreeBuffer ( pPublicKey );
    return pPublicKeyCopy;
}

dword ReSigner::GetKeyPairPublicKeyBitsSize ( byte* pKeyPair, dword dwKeyPairSize )
{
    dword dwPublicKeySize;
    byte* pPublicKey = GetKeyPairPublicKey ( pKeyPair, dwKeyPairSize, &dwPublicKeySize );
    if ( !pPublicKey )
        return 0;

    dword dwBitsSize = ((_RSAPUBKEY *)(pPublicKey + offsetof(PublicKeyBlob, PublicKey) + sizeof(BLOBHEADER)))->bitlen;
    delete[] pPublicKey;
    return dwBitsSize;
}

std::wstring ReSigner::GetKeyPairPublicKeyToken ( byte* pKeyPair, dword dwKeyPairSize )
{
    byte* pPublicKey;
    dword dwPublicKeySize;
    if ( !StrongNameGetPublicKey ( NULL, pKeyPair, dwKeyPairSize, &pPublicKey, &dwPublicKeySize ) )
        return std::wstring ();

    byte* pToken;
    dword dwTokenLength;
    if ( !StrongNameTokenFromPublicKey ( pPublicKey, dwPublicKeySize, &pToken, &dwTokenLength ) )
    {
        StrongNameFreeBuffer ( pPublicKey );
        return std::wstring ();
    }

    std::wstring wstrToken = BytesToHex ( pToken, dwTokenLength );
    StrongNameFreeBuffer ( pPublicKey );
    StrongNameFreeBuffer ( pToken );
    return wstrToken;
}

void ReSigner::ReplaceData ( byte* pLookIn, dword dwLookInLength, byte* pLookFor, dword dwLookForLength, byte* pReplaceBy )
{
    byte* pPos = pLookIn;
    while ( pPos < pLookIn + dwLookInLength )
    {
        pPos = std::search ( pPos, pLookIn + dwLookInLength, pLookFor, pLookFor + dwLookForLength );
        if ( pPos >= pLookIn + dwLookInLength )
            break;

        memcpy ( pPos, pReplaceBy, dwLookForLength );
        pPos += dwLookForLength;
    }
}

std::wstring ReSigner::BytesToHex ( byte* pData, dword dwSize )
{
    std::wstring wstrResult;
    wstrResult.resize ( 2 * dwSize );

    wchar_t* pwszByte = const_cast < wchar_t* > ( wstrResult.data () );
    for ( dword i = 0; i < dwSize; i++ )
    {
        wsprintf ( pwszByte, L"%02x", pData[i] );
        pwszByte += 2;
    }

    return wstrResult;
}
