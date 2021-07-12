#include "StdInc.h"

using namespace dotnetpe;

/*

    The purpose of the import hiding feature is to obfuscate all calls
    to methods of referenced types. It does this by creating a proxy delegate
    for every imported method, and replacing every call to an imported
    method by a call to the delegate. Then, at runtime, the delegates
    get assigned a function generated using reflection, which does nothing
    more than directly passing control to the original imported function.

    The delegates are static variables contained in their own class.
    A delegate type gets added per imported method signature. These delegate
    types are appended to the TypeDef meta table (i.e. they come last) and
    have no namespace. A calculation on each delegate's field name results
    in an index into the MemberRef table (which is completely intact).

*/

void AntiImportHiding::PreProcess ( const wchar_t* pwszFile )
{

}

void AntiImportHiding::Init ( const ProtectionStripperArgs& args )
{
    wprintf ( L"Resolving indirect imports...\n" );

    Module* pModule = args.m_pPE->GetStructureLayer ()->GetModule ();
    Type* pMultiCastDgType = pModule->GetReferencedType ( "System.MulticastDelegate" );
    if ( !pMultiCastDgType )
        return;

    // Start at the end of the TypeDef table and work upwards
    for ( int i = pModule->NumTypes () - 1; i >= 0; i-- )
    {
        // The proxy delegate classes are delegates and have no namespace
        TypeDef* pType = pModule->GetType ( i );
        if ( !pType->GetBaseType () || *pType->GetBaseType () != *pMultiCastDgType ||
             (pType->GetNamespace () && *pType->GetNamespace () != '\0') )
        {
            continue;
        }

        // Find the static constructor (which initializes the delegates with dynamically
        // created functions)
        MethodDef* pCctor = pType->GetMethod ( ".cctor" );
        if ( pCctor == NULL || !IsValidResolverCctor ( pCctor, (pType->Token() & 0x00FFFFFF) - 1 ) )
            break;

        MethodDef* pInvokeMethod = pType->GetMethod ( "Invoke" );
        assert ( pInvokeMethod );

        // Create the mapping for the fields
        for ( dword j = 0; j < pType->NumFields (); j++ )
        {
            Field* pField = pType->GetField ( j );
            if ( !pField->IsStatic () )
                continue;

            Type* pFieldType = pField->GetClass ();
            if ( !pFieldType || *pFieldType != *pType )
                continue;

            AddMapping ( pField->Token (), pField->GetName (), pInvokeMethod->Token () );
        }
    }
}

void AntiImportHiding::StripMethod ( const ProtectionStripperArgs& args )
{
    byte* pInstr = args.m_pMethod->Code ();
    byte* pCodeEnd = args.m_pMethod->Code () + args.m_pMethod->CodeSize ();

    ProxyStack_t stack;

    int iLength = 0;
    for ( ; pInstr < pCodeEnd; pInstr += iLength )
    {
        iLength = ILInstr::GetLength ( pInstr );

        if ( *pInstr == ILOPCODE_LDSFLD )
        {
            dword dwProxyToken = *(dword *)(pInstr + 1);
            ProxyInfos_t::iterator it = ms_ProxyInfos.find ( dwProxyToken );
            if ( it != ms_ProxyInfos.end () )
            {
                stack.push_back ( &it->second );
                memset ( pInstr, ILOPCODE_NOP, iLength );
            }
        }
        else if ( ILInstr::IsCall ( pInstr ) )
        {
            if ( !stack.empty () && *(dword *)(pInstr + 1) == stack.back ()->m_dwProxyInvokeMethodToken )
            {
                *pInstr = ( stack.back ()->m_bVirtual ? ILOPCODE_CALLVIRT : ILOPCODE_CALL );
                *(dword *)(pInstr + 1) = stack.back ()->m_dwImportMethodToken;
                stack.pop_back ();
            }
        }
        else if ( *pInstr == ILOPCODE_LEAVE || *pInstr == ILOPCODE_LEAVE_S || *pInstr == ILOPCODE_ENDFINALLY )
        {
            stack.clear ();
        }
    }
    assert ( stack.empty () );
}

void AntiImportHiding::Finish ( const ProtectionStripperArgs& args )
{

}

void AntiImportHiding::PostProcess ( const wchar_t* pwszFile )
{

}

void AntiImportHiding::AddMapping ( dword dwProxyToken, const char* pszProxyName, dword dwProxyInvokeMethodToken )
{
    static char cMap[] = { 
        '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07', '\x08', '\x0e', '\x0f', '\x10', '\x11', '\x12', '\x13', '\x14', '\x15', 
        '\x16', '\x17', '\x18', '\x19', '\x1a', '\x1b', '\x1c', '\x1d', '\x1e', '\x1f', '\x7f', '\x80', '\x81', '\x82', '\x83', '\x84', 
        '\x86', '\x87', '\x88', '\x89', '\x8a', '\x8b', '\x8c', '\x8d', '\x8e', '\x8f', '\x90', '\x91', '\x92', '\x93', '\x94', '\x95', 
        '\x96', '\x97', '\x98', '\x99', '\x9a', '\x9b', '\x9c', '\x9d', '\x9e', '\x9f', '\0'
    };

    int i = strlen ( pszProxyName );
    if ( i == 0 )
        return;

    i--;
    ProxyInfo proxy;
    proxy.m_bVirtual = false;
    proxy.m_dwProxyInvokeMethodToken = dwProxyInvokeMethodToken;

    dword dwImportIndex = 0;
    for ( ; i >= 0; i-- )
    {
        if ( pszProxyName[i] == '~' )
        {
            proxy.m_bVirtual = true;
            break;
        }

        const char* pszMapChar = strchr ( cMap, pszProxyName[i] );
        if ( pszMapChar )
            dwImportIndex = ( dwImportIndex * 0x3A ) + ( pszMapChar - cMap );
    }
    
    proxy.m_dwImportMethodToken = 0x0A000001 + dwImportIndex;
    ms_ProxyInfos [ dwProxyToken ] = proxy;
}

bool AntiImportHiding::IsValidResolverCctor ( MethodDef* pMethod, uint uiTypeIndex )
{
    if ( !pMethod )
        return false;

    byte* pCode = pMethod->Code ();
    byte* pCodeEnd = pCode + pMethod->CodeSize ();
    while ( pCode < pCodeEnd && *pCode == ILOPCODE_NOP )
    {
        pCode++;
    }

    switch ( pCodeEnd - pCode )
    {
        case 0x7:
            return uiTypeIndex <= 8 && pCode[0] == ILOPCODE_LDC_I4_0 + uiTypeIndex &&
                   pCode[1] == ILOPCODE_CALL && pCode[6] == ILOPCODE_RET;

        case 0x8:
            return uiTypeIndex <= 0xFF && pCode[0] == ILOPCODE_LDC_I4_S &&
                   pCode[1] == uiTypeIndex && pCode[2] == ILOPCODE_CALL && pCode[7] == ILOPCODE_RET;

        case 0xB:
            return pCode[0] == ILOPCODE_LDC_I4 && *(dword *)&pCode[1] == uiTypeIndex &&
                   pCode[5] == ILOPCODE_CALL && pCode[0xA] == ILOPCODE_RET;
    }
    return false;
}
