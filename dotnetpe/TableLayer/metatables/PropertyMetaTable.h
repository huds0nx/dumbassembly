#pragma once

namespace dotnetpe
{

    class PropertyMetaTable;

    class _PropertyMetaRecord : public MetaRecordBase < PropertyMetaTable >
    {
    public:
                            _PropertyMetaRecord             () {}
                            _PropertyMetaRecord             ( PropertyMetaTable* pTable, void* pRecord );

        word                GetFlags                        () const;
        const char*         GetName                         () const;
        const void*         GetType                         () const;
    };

    class PropertyMetaTable : public MetaTableBase < PropertyMetaTable, _PropertyMetaRecord >
    {
        friend _PropertyMetaRecord;

    public:
                            PropertyMetaTable               ();
        virtual             ~PropertyMetaTable              ();

        enum { TYPE = METATABLE_PROPERTY };
        virtual eType       GetType                         () const    { return (eType)TYPE; }

        virtual void        Init                            ( TableLayer* pTableLayer );

    private:
        WordMetaFieldReader         m_Flags;
        StringsIndexMetaFieldReader m_Name;
        BlobIndexMetaFieldReader    m_Type;
    };

}
