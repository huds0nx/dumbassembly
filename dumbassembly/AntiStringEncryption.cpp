#include "StdInc.h"

using namespace dotnetpe;

/*

    The string encryption is very simple - it only uses standard, widely known algorithms,
    nothing custom. It consists of three layers:
    - First, every string is encoded as UTF8 and then as base64. All strings are then
      laid out after each other, preceded by a variable-length encoded length.
    - This block is then compressed using raw deflate (headerless).
    - Finally, the compressed block is encrypted using regular DES. The key/IV are found
      in plaintext in the binary.

    At runtime, the entire block is decrypted and decompressed in one go when the program launches,
    in a static constructor.

    There are now two ways of getting to the strings at runtime, through a basic resolver and
    through an extended resolver. In both cases, the original ldstr instructions are replaced
    by a call to some method that takes an int and returns the string.

    First there is a class added to the assembly with a public, static method, the basic string
    resolver. It subtracts a global constant from the int argument - the value of this constant
    is found as text in the #US stream. The result is an offset into the decrypted and
    decompressed base64 block. The string length is read, and the base64 string is extracted,
    decoded from base64, and decoded from UTF8.

    A few locations use the basic string resolver directly, however most times an extended resolver
    is used. In this case, a class that uses the ldstr instruction has a static field added which stores
    a delegate. This field gets assigned a dynamically-generated method (Reflection.Emit) which
    does nothing more but pre-subtract the metadata index of the delegate field from the int
    argument, and then pass that on to the basic string resolver. (metadata index is the lower 24
    bits of the metadata token).
*/

AntiStringEncryption::AntiStringEncryption ()
{
    m_pBasicResolverType = NULL;
    m_pBasicResolverMethod = NULL;
    m_pExtendedResolverType = NULL;
    m_pExtendedResolverMethod = NULL;

    m_pStrings = NULL;
    m_dwStringsSize = 0;
    m_bFreeStrings = false;
}

AntiStringEncryption::~AntiStringEncryption ()
{
    if ( m_bFreeStrings && m_pStrings )
    {
        delete[] m_pStrings;
        m_pStrings = NULL;
        m_dwStringsSize = 0;
    }
}

void AntiStringEncryption::PreProcess ( const wchar_t* pwszFile )
{

}

void AntiStringEncryption::Init ( const ProtectionStripperArgs& args )
{
    if ( !FindBasicResolver ( args.m_pPE ) || !GetStringsResource ( args.m_pPE ) )
    {
        m_bStringsEncrypted = false;
        return;
    }
    m_bStringsEncrypted = true;

    wprintf ( L"Decrypting strings...\n" );

    FindExtendedResolvers ( args.m_pPE );
    m_iSubConst = GetSubtractionConst ( args.m_pPE );

    DecryptStringsResource ( args.m_pPE );

    // Initialize the new #US stream (keep original strings)
    MetaStream* pUSStream = args.m_pPE->GetUSMetaStream ();
    m_US.Reserve ( pUSStream->Size () + m_dwStringsSize );
    m_US.WriteBytes ( pUSStream->Data (), pUSStream->Size () );
}

void AntiStringEncryption::StripMethod ( const ProtectionStripperArgs& args )
{
    if ( !m_bStringsEncrypted )
        return;

    dword dwBasicResolverMethodToken = 0;
    dword dwExtendedResolverMethodToken = 0;
    if ( m_pBasicResolverMethod )
        dwBasicResolverMethodToken = m_pBasicResolverMethod->Token ();
    if ( m_pExtendedResolverMethod )
        dwExtendedResolverMethodToken = m_pExtendedResolverMethod->Token ();

    dword dwExtendedResolverFieldToken = 0;
    ExtendedResolvers_t::iterator it = m_ExtendedResolvers.find ( args.m_pType->Token () );
    if ( it != m_ExtendedResolvers.end () )
        dwExtendedResolverFieldToken = it->second;

    byte* pInstr = args.m_pMethod->Code ();
    byte* pCodeEnd = args.m_pMethod->Code () + args.m_pMethod->CodeSize ();

    int iLength = 0;
    for ( ; pInstr < pCodeEnd; pInstr += iLength )
    {
        iLength = ILInstr::GetLength ( pInstr );

        dword dwBase64Offset;
        dword dwInvokeLength;

        if ( ILInstr::IsConstIntLoad ( pInstr ) )
        {
            // Basic resolver?
            dword dwLoadLength = iLength;
            if ( *(pInstr + dwLoadLength) == ILOPCODE_CALL &&
                 *(dword *)(pInstr + dwLoadLength + 1) == dwBasicResolverMethodToken )
            {
                dwBase64Offset = ILInstr::GetConstIntLoadValue ( pInstr );
                dwInvokeLength = dwLoadLength + 5;
            }
            else
            {
                continue;
            }
        }
        else if ( *pInstr == ILOPCODE_LDSFLD && *(dword *)(pInstr + 1) == dwExtendedResolverFieldToken )
        {
            // Extended resolver?
            assert ( ILInstr::IsConstIntLoad ( pInstr + 5 ) );
            dword dwLoadLength = ILInstr::GetLength ( pInstr + 5 );
            assert ( *(pInstr + 5 + dwLoadLength) == ILOPCODE_CALL &&
                     *(dword *)(pInstr + 5 + dwLoadLength + 1) == dwExtendedResolverMethodToken );

            dwBase64Offset = ILInstr::GetConstIntLoadValue ( pInstr + 5 );
            dwInvokeLength = 5 + dwLoadLength + 5;

            dwBase64Offset -= dwExtendedResolverFieldToken & 0x00FFFFFF;
        }
        else
        {
            continue;
        }

        dwBase64Offset -= m_iSubConst;

        // Map the base64 offset to an offset into the new #US stream, creating the string
        // there if necessary
        dword dwUSOffset;
        StringOffsets_t::iterator it = m_StringOffsets.find ( dwBase64Offset );
        if ( it != m_StringOffsets.end () )
        {
            dwUSOffset = it->second;
        }
        else
        {
            // Base64 -> UTF8
            byte* pBase64 = &m_pStrings[dwBase64Offset];
            int iBase64Length = ReadVarLength ( pBase64 );
            std::string strUTF8;
            CryptoPP::StringSource ( pBase64, iBase64Length, true,
                new CryptoPP::Base64Decoder ( new CryptoPP::StringSink(strUTF8) ) );

            // UTF8 -> UTF16
            std::wstring wstr = Util::StringToWString ( strUTF8 );

            dwUSOffset = m_US.Size ();
            m_StringOffsets [ dwBase64Offset ] = dwUSOffset;
            m_US.WriteVarInt32 ( sizeof(wchar_t) * wstr.size () + 1 );
            m_US.WriteBytes ( wstr.c_str (), sizeof(wchar_t) * wstr.size () );
            m_US.Write ( (byte)1 );
        }

        // Overwrite resolver call with ldstr instruction
        memset ( pInstr, ILOPCODE_NOP, dwInvokeLength );
        *pInstr = ILOPCODE_LDSTR;
        *(dword *)(pInstr + 1) = mdtString | dwUSOffset;
        pInstr += dwInvokeLength;
        iLength = 0;
    }
}

void AntiStringEncryption::Finish ( const ProtectionStripperArgs& args )
{
    if ( m_bStringsEncrypted )
        args.m_pRebelFile->ReplaceStream ( "#US", m_US.Data (), m_US.Size () );
}

void AntiStringEncryption::PostProcess ( const wchar_t* pwszFile )
{

}

bool AntiStringEncryption::DecryptStringsResource ( DotNetPE* pPE )
{
    Resource* pResource = GetStringsResource ( pPE );
    if ( !pResource )
        return false;

    m_pStrings = ResourceDecryptor::DecryptResource ( pPE, pResource, &m_dwStringsSize );
    m_bFreeStrings = ( m_pStrings != pResource->Data () );
    return true;
}

bool AntiStringEncryption::FindBasicResolver ( DotNetPE* pPE )
{
    // Assume the basic resolver is the first method that makes use of base64 decoding
    // and string interning
    Module* pModule = pPE->GetModule ();
    TypeRef* pConvertType = pModule->GetReferencedType ( "System.Convert" );
    if ( !pConvertType )
        return false;

    MethodRef* pFromBase64Method = pConvertType->GetMethod ( "FromBase64String" );
    if ( !pFromBase64Method )
        return false;

    TypeRef* pStringType = pModule->GetReferencedType ( "System.String" );
    if ( !pStringType )
        return false;

    MethodRef* pInternMethod = pStringType->GetMethod ( "Intern" );
    if ( !pInternMethod )
        return false;

    Module::FindInfo findInfo;
    return pModule->FindNextInstr ( findInfo,
        [=] ( Module::FindInfo& info ) -> bool
        {
            if ( info.Type ()->NumMethods () != 2 )
                return false;

            byte* pInstr = info.Instruction ();
            if ( !m_pBasicResolverMethod )
            {
                if ( *pInstr == ILOPCODE_CALL && *(dword *)(pInstr + 1) == pFromBase64Method->Token () )
                {
                    m_pBasicResolverType = info.Type ();
                    m_pBasicResolverMethod = info.Method ();
                }
            }
            else if ( info.Method () != m_pBasicResolverMethod )
            {
                m_pBasicResolverType = NULL;
                m_pExtendedResolverMethod = NULL;
            }
            else if ( *pInstr == ILOPCODE_CALL && *(dword *)(pInstr + 1) == pInternMethod->Token () )
            {
                return true;
            }
            return false;
        }
    );
}

void AntiStringEncryption::FindExtendedResolvers ( DotNetPE* pPE )
{
    // Find the Delegate class that's used for the extended resolvers.
    // Do this by attempting to find the extended resolver setup method.
    // This is the first method that uses Type.GetFields and also references the
    // suspected extended resolver class.

    Module* pModule = pPE->GetModule ();

    MethodDef* pExtendedResolverSetupMethod = NULL;
    Module::FindInfo findInfo;
    pModule->FindNextInstr ( findInfo,
        [&] ( Module::FindInfo& findInfo ) -> bool
        {
            byte* pInstr = findInfo.Instruction ();
            if ( !pExtendedResolverSetupMethod )
            {
                if ( findInfo.Type ()->NumMethods () != 1 || findInfo.Type ()->NumFields () != 0 ||
                     *pInstr != ILOPCODE_CALLVIRT )
                    return false;

                dword dwMethodToken = *(dword *)(pInstr + 1);
                if ( (dwMethodToken & 0xFF000000) != mdtMemberRef )
                    return false;

                MethodRef method;
                method.Init ( pPE->GetStructureLayer (), dwMethodToken & 0x00FFFFFF );
                if ( method.GetType () && method.GetType ()->GetNamespace () && method.GetType ()->GetName () &&
                    !strcmp ( method.GetType ()->GetNamespace (), "System" ) &&
                    !strcmp ( method.GetType ()->GetName (), "Type" ) &&
                    !strcmp ( method.GetName (), "GetFields" ) )
                {
                    pExtendedResolverSetupMethod = findInfo.Method ();
                }
            }
            else if ( findInfo.Method () != pExtendedResolverSetupMethod )
            {
                pExtendedResolverSetupMethod = NULL;
            }
            else if ( *pInstr == ILOPCODE_LDTOKEN )
            {
                dword dwTypeToken = *(dword *)(pInstr + 1);
                if ( (dwTypeToken & 0xFF000000) != mdtTypeDef )
                    return false;

                TypeDef* pType = pModule->GetType ( (dwTypeToken & 0x00FFFFFF) - 1 );
                Type* pBaseType = pType->GetBaseType ();
                if ( pBaseType && pBaseType->GetNamespace () && pBaseType->GetName () &&
                    !strcmp ( pBaseType->GetNamespace (), "System" ) &&
                    !strcmp ( pBaseType->GetName (), "MulticastDelegate" ) )
                {
                    m_pExtendedResolverType = pType;
                    return true;
                }
            }
            return false;
        }
    );

    if ( !m_pExtendedResolverType )
        return;

    m_pExtendedResolverMethod = m_pExtendedResolverType->GetMethod ( "Invoke" );

    // For every class, see if it has a (static) field of this delegate class
    for ( int i = 0; i < pModule->NumTypes (); i++ )
    {
        TypeDef* pType = pModule->GetType ( i );
        for ( int j = 0; j < pType->NumFields (); j++ )
        {
            Field* pField = pType->GetField ( j );
            Type* pFieldType = pField->GetClass ();
            if ( pFieldType && *pFieldType == *m_pExtendedResolverType )
            {
                m_ExtendedResolvers [ pType->Token () ] = pField->Token ();
                break;
            }
        }
    }
}

int AntiStringEncryption::GetSubtractionConst ( dotnetpe::DotNetPE* pPE )
{
    if ( !m_pBasicResolverType )
        return 0;

    // So far two methods have been observed in which the subtraction constant is calculated.
    // In the first and most common, it is loaded as a string in the second ldstr in the basic resolver's .cctor.
    std::wstring wstrSubConst = GetBasicCctorString ( pPE, 1 );
    if ( !wstrSubConst.empty () )
        return _wtoi ( wstrSubConst.c_str () );

    // In the second method, the subtraction constant is calculated from the public key token
    dword dwSubtractionConst = 0;

    MethodDef* pCctor = m_pBasicResolverType->GetMethod ( ".cctor" );
    if ( pCctor )
    {
        byte* pInstr = pCctor->Code ();
        dword dwLdcLength = ILInstr::GetLength ( pInstr );
        if ( pCctor->CodeSize () != dwLdcLength + 6 || !ILInstr::IsConstIntLoad ( pInstr ) ||
             *(pInstr + dwLdcLength) != ILOPCODE_STSFLD || *(pInstr + dwLdcLength + 5) != ILOPCODE_RET )
        {
            return 0;
        }
        dword dwSubtractionConst = ILInstr::GetConstIntLoadValue ( pInstr );
    }
    
    byte ucPublicKeyToken[8];
    if ( pPE->GetAssembly ()->GetPublicKeyToken ( ucPublicKeyToken ) )
    {
        for ( int i = 0; i < 8; i += 2 )
        {
            dwSubtractionConst ^= (ucPublicKeyToken[i] << 8) | ucPublicKeyToken[i + 1];
        }
    }
    if ( pCctor )
        dwSubtractionConst ^= ((m_pBasicResolverMethod->Token () & 0x00FFFFFF) - 1) % 0xFFFF;

    return dwSubtractionConst;
}

Resource* AntiStringEncryption::GetStringsResource ( DotNetPE* pPE )
{
    // Try the mvid of the module as the resource name
    const GUID* pMvid = pPE->GetModule ()->GetMvid ();
    if ( !pMvid )
        return NULL;

    Resource* pResource = pPE->GetResource ( Util::GuidToString ( pMvid ).c_str () );
    if ( pResource )
        return pResource;

    // Try the 4th string in the basic resolver .cctor
    return pPE->GetResource ( Util::WStringToString ( GetBasicCctorString ( pPE, 3 ) ).c_str () );
}

std::wstring AntiStringEncryption::GetBasicCctorString ( DotNetPE* pPE, int iIndex )
{
    if ( !m_pBasicResolverType )
        return std::wstring ();

    MethodDef* pCctor = m_pBasicResolverType->GetMethod ( ".cctor" );
    if ( !pCctor )
        return std::wstring ();

    byte* pInstr = pCctor->Code ();
    byte* pInstrEnd = pCctor->Code () + pCctor->CodeSize ();
    int iLdstrCount = 0;
    dword dwStringOffset = 0;
    for ( ; pInstr < pInstrEnd; pInstr += ILInstr::GetLength ( pInstr ) )
    {
        if ( *pInstr != ILOPCODE_LDSTR )
            continue;

        if ( iLdstrCount == iIndex )
        {
            dwStringOffset = *(dword *)(pInstr + 1) & 0x00FFFFFF;
            break;
        }
        iLdstrCount++;
    }

    if ( !dwStringOffset )
        return std::wstring ();

    return pPE->GetUserString ( dwStringOffset );
}

int AntiStringEncryption::ReadVarLength ( byte*& ptr )
{
    MemoryStream stream ( ptr, 4, true );
    int iLength = stream.ReadVarInt32 ();
    ptr += stream.GetPosition ();
    return iLength;
}
