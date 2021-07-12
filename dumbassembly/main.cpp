#include "StdInc.h"

using namespace dotnetpe;

int wmain ( int argc, const wchar_t** argv )
{
    wprintf ( L"         DumbAssembly 0.5.8\n" );
    wprintf ( L"{smartassembly} unpacking tool by arc_\n" );
    wprintf ( L"--------------------------------------\n\n" );

    if ( argc < 2 )
    {
        printf ( "Usage: dumbassembly { infile.exe | infile.dll } [ keypair.snk ]\n" );
        return 1;
    }

    const wchar_t* pwszInFile = argv[1];
    std::wstring wstrOutFile ( pwszInFile );
    if ( wstrOutFile.find_last_of ( L'.' ) != std::wstring::npos )
        wstrOutFile.insert ( wstrOutFile.find_last_of ( L'.' ), L"_" );
    else
        wstrOutFile += L"_";

    const wchar_t* pwszKeyFile = NULL;
    if ( argc >= 3 )
        pwszKeyFile = argv[2];

    LARGE_INTEGER llPerfFreq;
    LARGE_INTEGER llStartTime;
    LARGE_INTEGER llEndTime;
    QueryPerformanceFrequency ( &llPerfFreq );
    QueryPerformanceCounter ( &llStartTime );
    
    DumbAssembly unprotector ( pwszInFile, wstrOutFile.c_str (), pwszKeyFile );
    bool success = unprotector.Process ();

    if ( success )
    {
        QueryPerformanceCounter ( &llEndTime );
        dword dwMS = (llEndTime.QuadPart - llStartTime.QuadPart) * 1000 / llPerfFreq.QuadPart;
        wprintf ( L"Completed unpacking in %d ms\n", dwMS );
    }

    return success ? 0 : 1;
}
