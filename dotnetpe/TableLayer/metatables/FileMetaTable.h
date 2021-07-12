#pragma once

namespace dotnetpe
{

    class FileMetaTable;

    class _FileMetaRecord : public MetaRecordBase < FileMetaTable >
    {
    public:
                            _FileMetaRecord             () {}
                            _FileMetaRecord             ( FileMetaTable* pTable, void* pRecord );

        dword               GetFlags                    () const;
        const char*         GetName                     () const;
        const void*         GetHashValue                () const;
    };

    class FileMetaTable : public MetaTableBase < FileMetaTable, _FileMetaRecord >
    {
        friend _FileMetaRecord;

    public:
                            FileMetaTable               ();
        virtual             ~FileMetaTable              ();

        enum { TYPE = METATABLE_FILE };
        virtual eType       GetType                     () const    { return (eType)TYPE; }

        virtual void        Init                        ( TableLayer* pTableLayer );

    private:
        DwordMetaFieldReader        m_Flags;
        StringsIndexMetaFieldReader m_Name;
        BlobIndexMetaFieldReader    m_HashValue;
    };

}
