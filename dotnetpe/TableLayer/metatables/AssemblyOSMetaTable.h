#pragma once

namespace dotnetpe
{

    class AssemblyOSMetaTable;

    class _AssemblyOSMetaRecord : public MetaRecordBase < AssemblyOSMetaTable >
    {
    public:
                            _AssemblyOSMetaRecord           () {}
                            _AssemblyOSMetaRecord           ( AssemblyOSMetaTable* pTable, void* pRecord );

        dword               GetOSPlatformID                 () const;
        dword               GetOSMajorVersion               () const;
        dword               GetOSMinorVersion               () const;
    };

    class AssemblyOSMetaTable : public MetaTableBase < AssemblyOSMetaTable, _AssemblyOSMetaRecord >
    {
        friend _AssemblyOSMetaRecord;

    public:
                            AssemblyOSMetaTable             ();
        virtual             ~AssemblyOSMetaTable            ();

        enum { TYPE = METATABLE_ASSEMBLYOS };
        virtual eType       GetType                         () const    { return (eType)TYPE; }

        virtual void        Init                            ( TableLayer* pTableLayer );

    private:
        DwordMetaFieldReader    m_OSPlatformID;
        DwordMetaFieldReader    m_OSMajorVersion;
        DwordMetaFieldReader    m_OSMinorVersion;
    };

}
