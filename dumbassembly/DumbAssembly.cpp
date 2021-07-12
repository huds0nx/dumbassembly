#include "StdInc.h"

using namespace dotnetpe;

DumbAssembly::DumbAssembly ( const wchar_t* pwszInFile, const wchar_t* pwszOutFile, const wchar_t* pwszKeyFile )
    : m_wstrInFile ( pwszInFile ), m_wstrOutFile ( pwszOutFile )
{
    if ( pwszKeyFile )
    {
        m_wstrKeyFile = pwszKeyFile;
    }
    else
    {
        m_wstrKeyFile = pwszInFile;
        m_wstrKeyFile += L".snk";
    }
}

bool DumbAssembly::Process ()
{
    if ( !CopyFile ( m_wstrInFile.c_str (), m_wstrOutFile.c_str (), FALSE ) )
    {
        wprintf ( L"Input file does not exist.\n" );
        return false;
    }

    PreProcess ();

    wprintf ( L"Loading input file...\n" );
    DotNetPE pe ( m_wstrOutFile.c_str () );
    if ( pe.IsBad () )
    {
        wprintf ( L"Failed to open file %s\n", m_wstrOutFile.c_str () );
        return false;
    }

    pe.FixNumberOfDirectories ();

	std::wstring wstrPoweredBy = GetSmartAssemblyVersion ( &pe );
    if ( wstrPoweredBy.empty () )
    {
        wprintf ( L"Input file is not protected with {smartassembly}.\n" );
        return false;
    }

    wprintf ( L"Assembly is [%ls].\n", wstrPoweredBy.c_str () );
    wprintf ( L"Module has %d methods.\n", pe.GetModule ()->NumMethods () );

    RebelFile rebel;
    RunProtectionStrippers ( &pe, &rebel );

    pe.Close ();

    if ( !rebel.Empty () )
    {
        wprintf ( L"Rebuilding with RebelDotNET...\n" );
        if ( !rebel.Run ( m_wstrOutFile.c_str () ) )
        {
            wprintf ( L"Failed to rebuild with RebelDotNET. Protection removal is not complete.\n" );
            return false;
        }
    }

    PostProcess ();

    return ReSignAssembly ();
}

void DumbAssembly::PreProcess ()
{
    IProtectionStripper** ppStrippers = GetProtectionStrippers ();
    for ( int i = 0; ppStrippers[i]; i++ )
    {
        ppStrippers[i]->PreProcess ( m_wstrOutFile.c_str () );
    }
}

void DumbAssembly::RunProtectionStrippers ( dotnetpe::DotNetPE* pPE, RebelFile* pRebel )
{
    IProtectionStripper** ppStrippers = GetProtectionStrippers ();
    ProtectionStripperArgs stripperArgs ( pPE );
    stripperArgs.m_pRebelFile = pRebel;

    Module* pModule = pPE->GetModule ();
    for ( int i = 0; ppStrippers[i]; i++ )
    {
        ppStrippers[i]->Init ( stripperArgs );

        if ( ppStrippers[i]->GetType () == IProtectionStripper::PER_METHOD )
        {
            for ( int j = 0; j < pModule->NumTypes (); j++ )
            {
                stripperArgs.m_pType = pModule->GetType ( j );
                for ( int k = 0; k < stripperArgs.m_pType->NumMethods (); k++ )
                {
                    stripperArgs.m_pMethod = stripperArgs.m_pType->GetMethod ( k );
                    //wprintf ( L"Method %08X\n", stripperArgs.m_pMethod->Token () );
                    if ( stripperArgs.m_pMethod->Code () )
                        ppStrippers[i]->StripMethod ( stripperArgs );
                }
            }
        }

        stripperArgs.m_pMethod = NULL;
        ppStrippers[i]->Finish ( stripperArgs );
    }
}

void DumbAssembly::PostProcess ()
{
    IProtectionStripper** ppStrippers = GetProtectionStrippers ();
    for ( int i = 0; ppStrippers[i]; i++ )
    {
        ppStrippers[i]->PostProcess ( m_wstrOutFile.c_str () );
    }
}

bool DumbAssembly::ReSignAssembly ()
{
    if ( !ReSigner::IsAssemblySigned ( m_wstrInFile.c_str () ) )
        return true;

    wprintf ( L"Re-signing with %ls...\n", m_wstrKeyFile.c_str () );

    dword dwOriginalPublicKeyBitsSize = ReSigner::GetAssemblyPublicKeyBitsSize ( m_wstrInFile.c_str () );
    
    dword dwNewKeyFileSize;
    byte* pNewKeyFile = Util::GetFileContent ( m_wstrKeyFile.c_str (), &dwNewKeyFileSize );
    if ( pNewKeyFile )
    {
        dword dwNewPublicKeyBitsSize = ReSigner::GetKeyPairPublicKeyBitsSize ( pNewKeyFile, dwNewKeyFileSize );
        delete[] pNewKeyFile;

        if ( dwNewPublicKeyBitsSize != dwOriginalPublicKeyBitsSize )
        {
            wprintf (
                L"Public key size mismatch! Original assembly is signed with %d-bit key,\n"
                L"specified key file is %d-bit. Unpacked assembly will not be re-signed.\n",
                dwOriginalPublicKeyBitsSize,
                dwNewPublicKeyBitsSize
            );
            return false;
        }
    }
    else
    {
        if ( !ReSigner::CreateStrongNameKeyFile ( m_wstrKeyFile.c_str (), dwOriginalPublicKeyBitsSize ) )
        {
            wprintf ( L"Failed to generate a key pair at %ls. Try generating one manually and specifying it in the command line arguments.\n", m_wstrKeyFile.c_str () );
            return false;
        }
    }

    ReSigner reSigner ( m_wstrKeyFile.c_str () );
    bool success = reSigner.UpdateAssembly ( m_wstrOutFile.c_str (), m_wstrInFile.c_str () );
    if ( !success )
        wprintf ( L"Re-signing failed!\n" );

    return success;
}

IProtectionStripper** DumbAssembly::GetProtectionStrippers () const
{
    static AntiCodeSplicing antiCodeSplicing;
    static AntiImportHiding antiImportHiding;
    static AntiStringEncryption antiStringEncryption;
    static AntiResourceEncryption antiResourceEncryption;

    static IProtectionStripper* strippers[] = {
        &antiCodeSplicing,
        &antiImportHiding,
        &antiStringEncryption,
        &antiResourceEncryption,
        NULL
    };
    return strippers;
}

std::wstring DumbAssembly::GetSmartAssemblyVersion ( DotNetPE* pPE ) const
{
    const char* ppszPoweredByAttributeNames[] = {
        "SmartAssembly.Attributes.PoweredByAttribute",
        "SmartAssembly.Attributes.PoweredByAttribute2",
        "SmartAssembly.Attributes.PoweredByAttribute3",
        NULL
    };

    const void* pPoweredBy = NULL;
    for ( const char** ppszAttrName = ppszPoweredByAttributeNames; *ppszAttrName; ppszAttrName++ )
    {
        pPoweredBy = pPE->GetAssembly ()->GetCustomAttributeData ( *ppszAttrName );
        if ( pPoweredBy )
            break;
    }
    
    if ( !pPoweredBy )
        return std::wstring ();

    MemoryStream stream ( pPoweredBy, 0x80, true );
    stream.ReadVarInt32 ();
    assert ( stream.ReadUInt16 () == 0x0001 );
    return stream.ReadVarUtf8String ();
}
