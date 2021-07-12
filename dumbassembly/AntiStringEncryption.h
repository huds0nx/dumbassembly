#pragma once

class AntiStringEncryption : public IProtectionStripper
{
public:
                            AntiStringEncryption    ();
                            ~AntiStringEncryption   ();

    eType                   GetType                 () const { return PER_METHOD; }

    void                    PreProcess              ( const wchar_t* pwszFile );

    void                    Init                    ( const ProtectionStripperArgs& args );
    void                    StripMethod             ( const ProtectionStripperArgs& args );
    void                    Finish                  ( const ProtectionStripperArgs& args );

    void                    PostProcess             ( const wchar_t* pwszFile );

private:
    bool                    DecryptStringsResource  ( dotnetpe::DotNetPE* pPE );
    bool                    FindBasicResolver       ( dotnetpe::DotNetPE* pPE );
    void                    FindExtendedResolvers   ( dotnetpe::DotNetPE* pPE );
    int                     GetSubtractionConst     ( dotnetpe::DotNetPE* pPE );

    dotnetpe::Resource*     GetStringsResource      ( dotnetpe::DotNetPE* pPE );
    
    std::wstring            GetBasicCctorString     ( dotnetpe::DotNetPE* pPE, int iIndex );
    int                     ReadVarLength           ( byte*& ptr );

    bool                    m_bStringsEncrypted;

    dotnetpe::TypeDef*      m_pBasicResolverType;
    dotnetpe::MethodDef*    m_pBasicResolverMethod;
    dotnetpe::TypeDef*      m_pExtendedResolverType;
    dotnetpe::MethodDef*    m_pExtendedResolverMethod;
    int                     m_iSubConst;

    byte*                   m_pStrings;
    dword                   m_dwStringsSize;
    bool                    m_bFreeStrings;

    typedef std::map < dword, dword > ExtendedResolvers_t;
    ExtendedResolvers_t     m_ExtendedResolvers;

    typedef std::map < dword, dword > StringOffsets_t;
    StringOffsets_t         m_StringOffsets;

    MemoryStream            m_US;
};
