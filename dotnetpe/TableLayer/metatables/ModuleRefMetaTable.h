#pragma once

namespace dotnetpe
{
    
    class ModuleRefMetaTable;

    class _ModuleRefMetaRecord : public MetaRecordBase < ModuleRefMetaTable >
    {
    public:
                            _ModuleRefMetaRecord            () {}
                            _ModuleRefMetaRecord            ( ModuleRefMetaTable* pTable, void* pRecord );

        const char*         GetName                         () const;
    };

    class ModuleRefMetaTable : public MetaTableBase < ModuleRefMetaTable, _ModuleRefMetaRecord >
    {
        friend _ModuleRefMetaRecord;

    public:
                            ModuleRefMetaTable              ();
        virtual             ~ModuleRefMetaTable             ();

        enum { TYPE = METATABLE_MODULEREF };
        virtual eType       GetType                         () const    { return (eType)TYPE; }

        virtual void        Init                            ( TableLayer* pTableLayer );

    private:
        StringsIndexMetaFieldReader m_Name;
    };

}
