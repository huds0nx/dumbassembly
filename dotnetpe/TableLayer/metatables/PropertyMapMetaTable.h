#pragma once

namespace dotnetpe
{

    class PropertyMapMetaTable;

    class _PropertyMapMetaRecord : public MetaRecordBase < PropertyMapMetaTable >
    {
    public:
                            _PropertyMapMetaRecord          () {}
                            _PropertyMapMetaRecord          ( PropertyMapMetaTable* pTable, void* pRecord );

        dword               GetParent                       () const;
        dword               GetPropertyList                 () const;
    };

    class PropertyMapMetaTable : public MetaTableBase < PropertyMapMetaTable, _PropertyMapMetaRecord >
    {
        friend _PropertyMapMetaRecord;

    public:
                            PropertyMapMetaTable            ();
        virtual             ~PropertyMapMetaTable           ();

        enum { TYPE = METATABLE_PROPERTYMAP };
        virtual eType       GetType                         () const    { return (eType)TYPE; }

        virtual void        Init                            ( TableLayer* pTableLayer );

    private:
        TypeDefIndexMetaFieldReader     m_Parent;
        PropertyIndexMetaFieldReader    m_PropertyList;
    };

}
