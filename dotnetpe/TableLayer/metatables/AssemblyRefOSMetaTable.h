#pragma once

namespace dotnetpe
{

    class AssemblyRefOSMetaTable;

    class _AssemblyRefOSMetaRecord : public MetaRecordBase < AssemblyRefOSMetaTable >
    {
    public:
                            _AssemblyRefOSMetaRecord        () {}
                            _AssemblyRefOSMetaRecord        ( AssemblyRefOSMetaTable* pTable, void* pRecord );

        dword               GetOSPlatformId                 () const;
        dword               GetOSMajorVersion               () const;
        dword               GetOSMinorVersion               () const;
        dword               GetAssemblyRef                  () const;
    };

    class AssemblyRefOSMetaTable : public MetaTableBase < AssemblyRefOSMetaTable, _AssemblyRefOSMetaRecord >
    {
        friend _AssemblyRefOSMetaRecord;

    public:
                            AssemblyRefOSMetaTable          ();
        virtual             ~AssemblyRefOSMetaTable         ();

        enum { TYPE = METATABLE_ASSEMBLYREFOS };
        virtual eType       GetType                         () const    { return (eType)TYPE; }

        virtual void        Init                            ( TableLayer* pTableLayer );

    private:
        DwordMetaFieldReader            m_OSPlatformId;
        DwordMetaFieldReader            m_OSMajorVersion;
        DwordMetaFieldReader            m_OSMinorVersion;
        AssemblyRefIndexMetaFieldReader m_AssemblyRef;
    };

}
