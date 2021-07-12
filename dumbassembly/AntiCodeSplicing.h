#pragma once

class AntiCodeSplicing : public IProtectionStripper
{
public:
                                ~AntiCodeSplicing       () {}

    eType                       GetType                 () const    { return PER_METHOD; }

    void                        PreProcess              ( const wchar_t* pwszFile );

    void                        Init                    ( const ProtectionStripperArgs& args );
    void                        StripMethod             ( const ProtectionStripperArgs& args );
    void                        Finish                  ( const ProtectionStripperArgs& args );

    void                        PostProcess             ( const wchar_t* pwszFile );

private:
    void                        FixPseudoConditionals   ( dotnetpe::MethodDef* pMethod );
    void                        RestoreSplices          ();

    dotnetpe::BasicBlockPool    m_BasicBlocks;
};
