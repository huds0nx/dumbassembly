#include "StdInc.h"

using namespace dotnetpe;

/*

    The protected assembly's resources are moved into a separate assembly
    which is compressed and encrypted in the same way and with the same
    key as the strings (see AntiStringEncryption).

    Since the original resources are no longer there, a ResourceResolve
    event is triggered whenever an attempt is made to load a resource.
    This causes the SmartAssembly code to load the resource assembly
    with Assembly.Load - again this does not succeed directly, and
    the AssemblyResolve event is triggered. At this point the resource
    assembly is decrypted in memory and loaded, and the originally
    requested resource is searched in it.

*/

void AntiResourceEncryption::PreProcess ( const wchar_t* pwszFile )
{
    
}

void AntiResourceEncryption::Init ( const ProtectionStripperArgs& args )
{
    if ( args.m_pPE->NumResources () == 0 )
        return;

    wprintf ( L"Decrypting and extracting resources...\n" );

    std::wstring wstrFolder = args.m_pPE->GetFileName ();
    if ( wstrFolder.find_last_of ( L'\\' ) != std::wstring::npos )
        wstrFolder.resize ( wstrFolder.find_last_of ( L'\\' ) + 1 );
    else
        wstrFolder = L"";

    std::string strStringsResourceName;
    const GUID* pMvid = args.m_pPE->GetModule ()->GetMvid ();
    if ( pMvid )
        strStringsResourceName = Util::GuidToString ( pMvid );

    for ( int i = 0; i < args.m_pPE->NumResources (); i++ )
    {
        Resource* pResource = args.m_pPE->GetResource ( i );
        if ( pResource->GetName ()[0] != '{' || pResource->GetName () == strStringsResourceName )
            continue;

        dword dwAssemblyImageSize;
        byte* pAssemblyImage = ResourceDecryptor::DecryptResource ( args.m_pPE, pResource, &dwAssemblyImageSize );
        DotNetPE resourcePE ( pAssemblyImage, dwAssemblyImageSize );
        if ( resourcePE.IsBad () )
        {
            if ( pAssemblyImage != pResource->Data () )
                delete[] pAssemblyImage;

            resourcePE.Close ();
            continue;
        }
        std::wstring wstrAssemblyName = Util::StringToWString ( resourcePE.GetAssembly ()->GetName () );
        resourcePE.Close ();

        std::wstring wstrAssemblyPath = wstrFolder + wstrAssemblyName + L".dll";
        FILE* pAssemblyFile = _wfopen ( wstrAssemblyPath.c_str (), L"wb+" );
        if ( !pAssemblyFile )
        {
            if ( pAssemblyImage != pResource->Data () )
                delete[] pAssemblyImage;

            wprintf ( L"Failed to extract %ls\n", wstrAssemblyPath.c_str () );
            continue;
        }
        fwrite ( pAssemblyImage, 1, dwAssemblyImageSize, pAssemblyFile );
        fclose ( pAssemblyFile );

        if ( pAssemblyImage != pResource->Data () )
            delete[] pAssemblyImage;

        if ( wstrAssemblyName == Util::StringToWString ( pResource->GetName () ) )
            m_wstrResourceAssemblyPath = wstrAssemblyPath;
    }
}

void AntiResourceEncryption::StripMethod ( const ProtectionStripperArgs& args )
{

}

void AntiResourceEncryption::Finish ( const ProtectionStripperArgs& args )
{

}

void AntiResourceEncryption::PostProcess ( const wchar_t* pwszFile )
{
    if ( m_wstrResourceAssemblyPath.empty () )
        return;

    wprintf ( L"Merging decrypted resources into assembly...\n" );
    if ( MergeResources ( pwszFile, m_wstrResourceAssemblyPath.c_str () ) )
        DeleteFile ( m_wstrResourceAssemblyPath.c_str () );
    else
        wprintf ( L"Failed to merge resource %ls\n", m_wstrResourceAssemblyPath.c_str () );

    m_wstrResourceAssemblyPath.clear ();
}

bool AntiResourceEncryption::MergeResources ( const wchar_t* pwszMergeIntoFile, const wchar_t* pwszFromFile )
{
    STARTUPINFO startInfo;
    memset ( &startInfo, 0, sizeof(startInfo) );
    startInfo.cb = sizeof(startInfo);
    PROCESS_INFORMATION procInfo;

    std::wstring wstrCmdLine = L"ResMerge.exe \"";
    wstrCmdLine += pwszFromFile;
    wstrCmdLine += L"\" \"";
    wstrCmdLine += pwszMergeIntoFile;
    wstrCmdLine += L"\"";

    if ( !CreateProcess ( NULL, const_cast < wchar_t* > ( wstrCmdLine.c_str () ), NULL, NULL,
            false, 0, NULL, NULL, &startInfo, &procInfo ) )
    {
        return false;
    }

    WaitForSingleObject ( procInfo.hThread, INFINITE );
    CloseHandle ( procInfo.hThread );
    CloseHandle ( procInfo.hProcess );
    return true;
}
