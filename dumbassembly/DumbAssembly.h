#pragma once

class DumbAssembly
{
public:
                                DumbAssembly                ( const wchar_t* pwszInFile, const wchar_t* pwszOutFile, const wchar_t* pwszKeyFile );

    bool                        Process                     ();

private:
    void                        PreProcess                  ();
    void                        RunProtectionStrippers      ( dotnetpe::DotNetPE* pPE, RebelFile* pRebel );
    void                        PostProcess                 ();

    bool                        ReSignAssembly              ();
    IProtectionStripper**       GetProtectionStrippers      () const;
    std::wstring                GetSmartAssemblyVersion     ( dotnetpe::DotNetPE* pPE ) const;

    std::wstring                m_wstrInFile;
    std::wstring                m_wstrOutFile;
    std::wstring                m_wstrKeyFile;
};
