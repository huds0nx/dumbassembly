#pragma once

namespace dotnetpe
{

    class EventMetaTable;

    class _EventMetaRecord : public MetaRecordBase < EventMetaTable >
    {
    public:
                            _EventMetaRecord            () {}
                            _EventMetaRecord            ( EventMetaTable* pTable, void* pRecord );

        word                GetEventFlags               () const;
        const char*         GetName                     () const;
        dword               GetEventType                ( MetaTable*& pTable ) const;
    };

    class EventMetaTable : public MetaTableBase < EventMetaTable, _EventMetaRecord >
    {
        friend _EventMetaRecord;

    public:
                            EventMetaTable              ();
        virtual             ~EventMetaTable             ();

        enum { TYPE = METATABLE_EVENT };
        virtual eType       GetType                     () const    { return (eType)TYPE; }

        virtual void        Init                        ( TableLayer* pTableLayer );

    private:
        WordMetaFieldReader                 m_EventFlags;
        StringsIndexMetaFieldReader         m_Name;
        TypeDefOrRefIndexMetaFieldReader    m_EventType;
    };

}
