#pragma once

namespace dotnetpe
{

    class AssemblyMetaTable;

    class _AssemblyMetaRecord : public MetaRecordBase < AssemblyMetaTable >
    {
    public:
                            _AssemblyMetaRecord             () {}
                            _AssemblyMetaRecord             ( AssemblyMetaTable* pTable, void* pRecord );

        dword               GetHashAlgId                    () const;
        word                GetMajorVersion                 () const;
        word                GetMinorVersion                 () const;
        word                GetBuildNumber                  () const;
        word                GetRevisionNumber               () const;
        dword               GetFlags                        () const;
        const void*         GetPublicKey                    () const;
        const char*         GetName                         () const;
        const char*         GetCulture                      () const;
    };

    class AssemblyMetaTable : public MetaTableBase < AssemblyMetaTable, _AssemblyMetaRecord >
    {
        friend _AssemblyMetaRecord;

    public:
                            AssemblyMetaTable               ();
        virtual             ~AssemblyMetaTable              ();

        enum { TYPE = METATABLE_ASSEMBLY };
        virtual eType       GetType                         () const    { return (eType)TYPE; }

        virtual void        Init                            ( TableLayer* pTableLayer );

    private:
        DwordMetaFieldReader        m_HashAlgId;
        WordMetaFieldReader         m_MajorVersion;
        WordMetaFieldReader         m_MinorVersion;
        WordMetaFieldReader         m_BuildNumber;
        WordMetaFieldReader         m_RevisionNumber;
        DwordMetaFieldReader        m_Flags;
        BlobIndexMetaFieldReader    m_PublicKey;
        StringsIndexMetaFieldReader m_Name;
        StringsIndexMetaFieldReader m_Culture;
    };

}

