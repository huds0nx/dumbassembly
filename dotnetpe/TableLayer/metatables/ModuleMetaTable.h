#pragma once

namespace dotnetpe
{

    class ModuleMetaTable;

    class _ModuleMetaRecord : public MetaRecordBase < ModuleMetaTable >
    {
    public:
                                _ModuleMetaRecord           () {}
                                _ModuleMetaRecord           ( ModuleMetaTable* pTable, void* pRecord );

        word                    GetGeneration               () const;
        const char*             GetName                     () const;
        const GUID*             GetMvid                     () const;
        const GUID*             GetEncId                    () const;
        const GUID*             GetEncBaseId                () const;
    };

    class ModuleMetaTable : public MetaTableBase < ModuleMetaTable, _ModuleMetaRecord >
    {
        friend _ModuleMetaRecord;

    public:
                                ModuleMetaTable             ();
        virtual                 ~ModuleMetaTable            ();

        enum { TYPE = METATABLE_MODULE };
        virtual eType           GetType                     () const    { return (eType)TYPE; }

        virtual void            Init                        ( TableLayer* pTableLayer );

    private:
        WordMetaFieldReader         m_Generation;
        StringsIndexMetaFieldReader m_Name;
        GUIDIndexMetaFieldReader    m_Mvid;
        GUIDIndexMetaFieldReader    m_EncId;
        GUIDIndexMetaFieldReader    m_EncBaseId;
    };

}
