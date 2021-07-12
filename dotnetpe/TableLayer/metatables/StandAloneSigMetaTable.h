#pragma once

namespace dotnetpe
{

    class StandAloneSigMetaTable;

    class _StandAloneSigMetaRecord : public MetaRecordBase < StandAloneSigMetaTable >
    {
    public:
                                _StandAloneSigMetaRecord    () {}
                                _StandAloneSigMetaRecord    ( StandAloneSigMetaTable* pTable, void* pRecord );

        const void*             GetSignature                () const;
    };

    class StandAloneSigMetaTable : public MetaTableBase < StandAloneSigMetaTable, _StandAloneSigMetaRecord >
    {
        friend _StandAloneSigMetaRecord;

    public:
                                StandAloneSigMetaTable      ();
        virtual                 ~StandAloneSigMetaTable     ();

        enum { TYPE = METATABLE_STANDALONESIG };
        virtual eType           GetType                     () const    { return (eType)TYPE; }

        virtual void            Init                        ( TableLayer* pTableLayer );

    private:
        BlobIndexMetaFieldReader    m_Signature;
    };

}
