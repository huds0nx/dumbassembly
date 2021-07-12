#include "StdInc.h"

using namespace dotnetpe;

byte* ResourceDecryptor::DecryptResource ( DotNetPE* pPE, const char* pszResourceName, dword* pdwSize )
{
    Resource* pResource = pPE->GetResource ( pszResourceName );
    if ( !pResource )
        return NULL;

    return DecryptResource ( pPE, pszResourceName, pdwSize );
}

byte* ResourceDecryptor::DecryptResource ( DotNetPE* pPE, Resource* pResource, dword* pdwSize )
{
    byte* pData = (byte *)pResource->Data ();
    dword dwSize = pResource->Size ();

    bool bFirst = true;
    while ( true )
    {
        byte* pNewData = DecryptionPass ( pPE, pData, dwSize );
        if ( !pNewData )
            break;

        if ( bFirst )
            bFirst = false;
        else
            delete[] pData;

        pData = pNewData;
    }

    if ( pdwSize )
        *pdwSize = dwSize;

    return pData;
}

byte* ResourceDecryptor::DecryptionPass ( DotNetPE* pPE, byte* pData, dword& dwSize )
{
    byte* pResult = NULL;

    if ( dwSize >= sizeof(EncryptedResourceHeader) )
    {
        EncryptedResourceHeader* pHeader = reinterpret_cast < EncryptedResourceHeader* > ( pData );
        if ( memcmp ( pHeader->m_cMagic, "{z}", 3 ) == 0 )
        {
            pData += 4;
            dwSize -= 4;

            switch ( pHeader->m_ucVersion )
            {
                case 1:
                {
                    dword dwTotalUncompressed = *(dword *)pData;
                    pData += 4;
                    pResult = new byte[dwTotalUncompressed];
                    dword dwUncompressed = 0;
                    while ( dwUncompressed < dwTotalUncompressed )
                    {
                        dword dwIn = *(dword *)pData;
                        dword dwOut = *(dword *)(pData + 4);
                        pData += 8;
                    
                        assert ( dwUncompressed + dwOut <= dwTotalUncompressed );
                        RawInflate ( pResult + dwUncompressed, dwOut, pData, dwIn );
                    
                        pData += dwIn;
                        dwUncompressed += dwOut;
                    }
                    dwSize = dwTotalUncompressed;
                    break;
                }

                case 2:
                {
                    KeyCandidates_t keyCandidates = GetKeyCandidates ( pPE );
                    assert ( !keyCandidates.empty () );

                    int iKeyIndex = 0;
                    while ( iKeyIndex < keyCandidates.size () )
                    {
                        byte ucTestDecryption[8];
                        CryptoPP::CBC_Mode < CryptoPP::DES >::Decryption decryptor (
                            keyCandidates[iKeyIndex].m_pKey, 8, keyCandidates[iKeyIndex].m_pIV );
                        decryptor.ProcessData ( ucTestDecryption, pData, 8 );
                        if ( !memcmp ( ucTestDecryption, "{z}\x01", 4 ) )
                            break;

                        iKeyIndex++;
                    }
                    assert ( iKeyIndex < keyCandidates.size () );

                    pResult = new byte[dwSize];
                    CryptoPP::CBC_Mode < CryptoPP::DES >::Decryption decryptor (
                        keyCandidates[iKeyIndex].m_pKey, 8, keyCandidates[iKeyIndex].m_pIV );
                    decryptor.ProcessData ( pResult, pData, dwSize );
                    break;
                }
            }
        }
    }

    return pResult;
}

ResourceDecryptor::KeyCandidates_t ResourceDecryptor::GetKeyCandidates ( DotNetPE* pPE )
{
    // Find pairs of 8-byte valuetype fields
    KeyCandidates_t candidates;

    FieldRVAMetaTable* pFieldRVATable = (FieldRVAMetaTable *)pPE->GetMetaTable ( MetaTable::METATABLE_FIELDRVA );
    if ( pFieldRVATable->GetNumRecords () < 2 )
        return candidates;

    for ( dword i = 1; i < pFieldRVATable->GetNumRecords (); i++ )
    {
        FieldRVAMetaTable::Record keyRvaRec = pFieldRVATable->GetRecord ( i );
        FieldRVAMetaTable::Record ivRvaRec = pFieldRVATable->GetRecord ( i + 1 );

        Field keyField;
        Field ivField;
        keyField.Init ( pPE->GetStructureLayer (), keyRvaRec.GetField () & 0x00FFFFFF );
        ivField.Init ( pPE->GetStructureLayer (), ivRvaRec.GetField () & 0x00FFFFFF );
        if ( IsField8Bytes ( keyField ) && IsField8Bytes ( ivField ) )
            candidates.push_back ( KeyCandidate ( (byte *)keyField.GetData(), (byte *)ivField.GetData() ) );
    }

    return candidates;
}

bool ResourceDecryptor::IsField8Bytes ( dotnetpe::Field& field )
{
    if ( field.GetTypeKind () == ELEMENT_TYPE_I8 || field.GetTypeKind () == ELEMENT_TYPE_U8 )
        return true;

    return field.GetTypeKind () == ELEMENT_TYPE_VALUETYPE &&
           field.GetClass () &&
           field.GetClass ()->IsTypeDef () &&
           dynamic_cast < TypeDef* > ( field.GetClass () )->GetSize () == 8;
}

void ResourceDecryptor::RawInflate ( byte* pUncompressed, dword dwUncompressedLength, byte* pCompressed, dword dwCompressedLength )
{
    z_stream stream;
    stream.next_in = pCompressed;
    stream.avail_in = dwCompressedLength;
    stream.next_out = pUncompressed;
    stream.avail_out = dwUncompressedLength;
    stream.zalloc = NULL;
    stream.zfree = NULL;

    inflateInit2 ( &stream, -15 );
    inflate ( &stream, Z_FINISH );
    inflateEnd ( &stream );
}
