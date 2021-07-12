#pragma once

class RebelFile
{
public:
                            RebelFile               ();
                            ~RebelFile              ();

    void                    ReplaceStream           ( const char* pszName, const void* pData, dword dwSize );
    void                    ReplaceMethod           ( dword dwToken, const void* pCode, dword dwCodeSize, const void* pEH, dword dwEHSize );

    int                     NumStreams              () const    { return m_StreamInfos.size (); }
    int                     NumMethods              () const    { return m_MethodInfos.size (); }
    bool                    HasStreams              () const    { return !m_StreamInfos.empty (); }
    bool                    HasMethods              () const    { return !m_MethodInfos.empty (); }
    bool                    Empty                   () const    { return !HasStreams () && !HasMethods (); }

    bool                    Write                   ( const wchar_t* pwszFile ) const;
    bool                    Run                     ( const wchar_t* pwszFileToRebuild ) const;
    bool                    Run                     ( const wchar_t* pwszFileToRebuild, const wchar_t* pwszOutFile ) const;

private:
    struct StreamInfo
    {
        std::string     m_strName;
        const void*     m_pData;
        dword           m_dwSize;
    };
    typedef std::vector < StreamInfo > StreamInfos_t;
    StreamInfos_t       m_StreamInfos;

    struct MethodInfo
    {
        dword           m_dwToken;
        const void*     m_pCode;
        dword           m_dwCodeSize;
        const void*     m_pEH;
        dword           m_dwEHSize;
    };
    typedef std::vector < MethodInfo > MethodInfos_t;
    MethodInfos_t       m_MethodInfos;
};
