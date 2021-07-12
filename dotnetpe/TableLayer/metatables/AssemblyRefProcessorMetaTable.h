#pragma once

namespace dotnetpe
{

    class AssemblyRefProcessorMetaTable;

    class _AssemblyRefProcessorMetaRecord : public MetaRecordBase < AssemblyRefProcessorMetaTable >
    {
    public:
                        _AssemblyRefProcessorMetaRecord         () {}
                        _AssemblyRefProcessorMetaRecord         ( AssemblyRefProcessorMetaTable* pTable, void* pRecord );

        dword           GetProcessor                            () const;
        dword           GetAssemblyRef                          () const;
    };

    class AssemblyRefProcessorMetaTable : public MetaTableBase < AssemblyRefProcessorMetaTable, _AssemblyRefProcessorMetaRecord >
    {
        friend _AssemblyRefProcessorMetaRecord;

    public:
                        AssemblyRefProcessorMetaTable           ();
        virtual         ~AssemblyRefProcessorMetaTable          ();

        enum { TYPE = METATABLE_ASSEMBLYREFPROCESSOR };
        virtual eType   GetType                                 () const    { return (eType)TYPE; }

        virtual void    Init                                    ( TableLayer* pTableLayer );

    private:
        DwordMetaFieldReader            m_Processor;
        AssemblyRefIndexMetaFieldReader m_AssemblyRef;
    };

}
