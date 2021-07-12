#pragma once

namespace dotnetpe
{

    class AssemblyRefMetaTable;

    class _AssemblyRefMetaRecord : public MetaRecordBase < AssemblyRefMetaTable >
    {
    public:
                            _AssemblyRefMetaRecord      () {}
                            _AssemblyRefMetaRecord      ( AssemblyRefMetaTable* pTable, void* pRecord );

        word                GetMajorVersion             () const;
        word                GetMinorVersion             () const;
        word                GetBuildNumber              () const;
        word                GetRevisionNumber           () const;
        dword               GetFlags                    () const;
        const void*         GetPublicKeyOrToken         () const;
        const char*         GetName                     () const;
        const char*         GetCulture                  () const;
        const void*         GetHashValue                () const;
    };

    class AssemblyRefMetaTable : public MetaTableBase < AssemblyRefMetaTable, _AssemblyRefMetaRecord >
    {
        friend _AssemblyRefMetaRecord;

    public:
                            AssemblyRefMetaTable        ();
        virtual             ~AssemblyRefMetaTable       ();

        enum { TYPE = METATABLE_ASSEMBLYREF };
        virtual eType       GetType                     () const    { return (eType)TYPE; }

        virtual void        Init                        ( TableLayer* pTableLayer );

    private:
        WordMetaFieldReader         m_MajorVersion;
        WordMetaFieldReader         m_MinorVersion;
        WordMetaFieldReader         m_BuildNumber;
        WordMetaFieldReader         m_RevisionNumber;
        DwordMetaFieldReader        m_Flags;
        BlobIndexMetaFieldReader    m_PublicKeyOrToken;
        StringsIndexMetaFieldReader m_Name;
        StringsIndexMetaFieldReader m_Culture;
        BlobIndexMetaFieldReader    m_HashValue;
    };

}
