#pragma once

class AntiImportHiding : public IProtectionStripper
{
public:
                                ~AntiImportHiding       () {}

    eType                       GetType                 () const    { return PER_METHOD; }

    void                        PreProcess              ( const wchar_t* pwszFile );

    void                        Init                    ( const ProtectionStripperArgs& args );
    void                        StripMethod             ( const ProtectionStripperArgs& args );
    void                        Finish                  ( const ProtectionStripperArgs& args );

    void                        PostProcess             ( const wchar_t* pwszFile );

private:
    void                        AddMapping              ( dword dwProxyToken, const char* pszProxyName, dword dwProxyInvokeMethodToken );
    bool                        IsValidResolverCctor    ( dotnetpe::MethodDef* pMethod, uint uiTypeIndex );

    struct ProxyInfo
    {
        dword                   m_dwProxyInvokeMethodToken;
        dword                   m_dwImportMethodToken;
        bool                    m_bVirtual;
    };
    typedef std::map < dword, ProxyInfo > ProxyInfos_t;
    ProxyInfos_t                ms_ProxyInfos;

    typedef std::vector < ProxyInfo* > ProxyStack_t;
};
