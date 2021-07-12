#pragma once

class AntiResourceEncryption : public IProtectionStripper
{
public:
                            ~AntiResourceEncryption         () {}

    eType                   GetType                         () const    { return PER_MODULE; }

    void                    PreProcess                      ( const wchar_t* pwszFile );

    void                    Init                            ( const ProtectionStripperArgs& args );
    void                    StripMethod                     ( const ProtectionStripperArgs& args );
    void                    Finish                          ( const ProtectionStripperArgs& args );

    void                    PostProcess                     ( const wchar_t* pwszFile );

private:
    bool                    MergeResources                  ( const wchar_t* pwszMergeIntoFile, const wchar_t* pwszFromFile );

    std::wstring            m_wstrResourceAssemblyPath;
};
