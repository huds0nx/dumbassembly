#pragma once

namespace dotnetpe
{

    class AssemblyProcessorMetaTable;

    class _AssemblyProcessorMetaRecord : public MetaRecordBase < AssemblyProcessorMetaTable >
    {
    public:
                            _AssemblyProcessorMetaRecord    () {}
                            _AssemblyProcessorMetaRecord    ( AssemblyProcessorMetaTable* pTable, void* pRecord );

        dword               GetProcessor                    () const;
    };

    class AssemblyProcessorMetaTable : public MetaTableBase < AssemblyProcessorMetaTable, _AssemblyProcessorMetaRecord >
    {
        friend _AssemblyProcessorMetaRecord;

    public:
                            AssemblyProcessorMetaTable      ();
        virtual             ~AssemblyProcessorMetaTable     ();

        enum { TYPE = METATABLE_ASSEMBLYPROCESSOR };
        virtual eType       GetType                         () const    { return (eType)TYPE; }

        virtual void        Init                            ( TableLayer* pTableLayer );

    private:
        DwordMetaFieldReader    m_Processor;
    };

}
