#pragma once

namespace dotnetpe
{

    class EventMapMetaTable;

    class _EventMapMetaRecord : public MetaRecordBase < EventMapMetaTable >
    {
    public:
                            _EventMapMetaRecord     () {}
                            _EventMapMetaRecord     ( EventMapMetaTable* pTable, void* pRecord );

        dword               GetParent               () const;
        dword               GetEventList            () const;
    };

    class EventMapMetaTable : public MetaTableBase < EventMapMetaTable, _EventMapMetaRecord >
    {
        friend _EventMapMetaRecord;

    public:
                            EventMapMetaTable       ();
        virtual             ~EventMapMetaTable      ();

        enum { TYPE = METATABLE_EVENTMAP };
        virtual eType       GetType                 () const    { return (eType)TYPE; }

        virtual void        Init                    ( TableLayer* pTableLayer );

    private:
        TypeDefIndexMetaFieldReader m_Parent;
        EventIndexMetaFieldReader   m_EventList;
    };

}
