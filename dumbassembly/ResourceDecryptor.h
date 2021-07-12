#pragma once

class ResourceDecryptor
{
public:
    static byte*            DecryptResource         ( dotnetpe::DotNetPE* pPE, const char* pszResourceName, dword* pdwSize );
    static byte*            DecryptResource         ( dotnetpe::DotNetPE* pPE, dotnetpe::Resource* pResource, dword* pdwSize );

private:
    struct EncryptedResourceHeader
    {
        char                m_cMagic[3];
        byte                m_ucVersion;
    };

    struct KeyCandidate
    {
                            KeyCandidate            ( const byte* pKey, const byte* pIV )
        {
            m_pKey = pKey;
            m_pIV = pIV;
        }

        const byte*         m_pKey;
        const byte*         m_pIV;
    };
    typedef std::vector < KeyCandidate > KeyCandidates_t;

    static byte*            DecryptionPass          ( dotnetpe::DotNetPE* pPE, byte* pData, dword& dwSize );
    static KeyCandidates_t  GetKeyCandidates        ( dotnetpe::DotNetPE* pPE );
    static bool             IsField8Bytes           ( dotnetpe::Field& field );
    static void             RawInflate              ( byte* pUncompressed, dword dwUncompressedSize, byte* pCompressed, dword dwCompressedSize );
};
