#pragma once

class ReSigner
{
public:
                            ReSigner                        ( const wchar_t* pwszKeyFilePath );
                            ~ReSigner                       ();

    static bool             IsAssemblySigned                ( const wchar_t* pwszAssemblyPath );
    bool                    UpdateAssembly                  ( const wchar_t* pwszPatchedAssemblyPath, const wchar_t* pwszOriginalAssemblyPath );

    static bool             CreateStrongNameKeyFile         ( const wchar_t* pwszKeyFilePath, int iKeySize );

    static byte*            GetAssemblyPublicKey            ( const wchar_t* pwszAssemblyPath, dword* pdwPublicKeySize );
    static dword            GetAssemblyPublicKeyBitsSize    ( const wchar_t* pwszAssemblyPath );
    static std::wstring     GetAssemblyPublicKeyToken       ( const wchar_t* pwszAssemblyPath );

    static byte*            GetKeyPairPublicKey             ( byte* pKeyPair, dword dwKeyPairSize, dword* pdwPublicKeySize );
    static dword            GetKeyPairPublicKeyBitsSize     ( byte* pKeyPair, dword dwKeyPairSize );
    static std::wstring     GetKeyPairPublicKeyToken        ( byte* pKeyPair, dword dwKeyPairSize );

private:
    void                    UpdatePublicKeyTokenStrings     ( dotnetpe::DotNetPE* pPE, const wchar_t* pwszOriginalAssemblyPath );
    void                    UpdatePublicKeyArrays           ( dotnetpe::DotNetPE* pPE, const wchar_t* pwszOriginalAssemblyPath );

    static void             ReplaceData                     ( byte* pLookIn, dword dwLookInLength, byte* pLookFor, dword dwLookForLength, byte* pReplaceBy );
    static std::wstring     BytesToHex                      ( byte* pData, dword dwSize );

    std::wstring            m_wstrKeyFilePath;
    byte*                   m_pKeys;
    dword                   m_dwKeysSize;
};
