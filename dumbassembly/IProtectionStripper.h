#pragma once

struct ProtectionStripperArgs
{
public:
                            ProtectionStripperArgs  ()
    {
        m_pPE = NULL;
        m_pType = NULL;
        m_pMethod = NULL;
        m_pRebelFile = NULL;
    }

                            ProtectionStripperArgs ( dotnetpe::DotNetPE* pPE )
    {
        m_pPE = pPE;
        m_pType = NULL;
        m_pMethod = NULL;
        m_pRebelFile = NULL;
    }

    dotnetpe::DotNetPE*     m_pPE;
    dotnetpe::TypeDef*      m_pType;
    dotnetpe::MethodDef*    m_pMethod;
    RebelFile*              m_pRebelFile;
};

class IProtectionStripper
{
public:
    virtual                 ~IProtectionStripper    () {}

    enum eType
    {
        PER_MODULE,
        PER_METHOD
    };
    virtual eType           GetType                 () const = 0;

    virtual void            PreProcess              ( const wchar_t* pwszFile ) = 0;

    virtual void            Init                    ( const ProtectionStripperArgs& args ) = 0;
    virtual void            StripMethod             ( const ProtectionStripperArgs& args ) = 0;
    virtual void            Finish                  ( const ProtectionStripperArgs& args ) = 0;

    virtual void            PostProcess             ( const wchar_t* pwszFile ) = 0;
};
