#pragma once

namespace dotnetpe
{

    class DotNetPE;

    class MetaTable
    {
    public:
                                MetaTable                       ();
        virtual                 ~MetaTable                      ();

        virtual void            Init                            ( TableLayer* pTableLayer );

        int                     GetNumRecords                   () const        { return m_iNumRecords; }
        void                    SetNumRecords                   ( int iNum )    { m_iNumRecords = iNum; }

        void*                   GetRecordsBase                  () const        { return m_pRecords; }
        void                    SetRecordsBase                  ( void* pRecords );

        int                     GetRecordSize                   () const;

        enum eType
        {
            METATABLE_MODULE                    = 0x00,
            METATABLE_TYPEREF                   = 0x01,
            METATABLE_TYPEDEF                   = 0x02,
            METATABLE_FIELD                     = 0x04,
            METATABLE_METHODDEF                 = 0x06,
            METATABLE_PARAM                     = 0x08,
            METATABLE_INTERFACEIMPL             = 0x09,
            METATABLE_MEMBERREF                 = 0x0A,
            METATABLE_CONSTANT                  = 0x0B,
            METATABLE_CUSTOMATTRIBUTE           = 0x0C,
            METATABLE_FIELDMARSHAL              = 0x0D,
            METATABLE_DECLSECURITY              = 0x0E,
            METATABLE_CLASSLAYOUT               = 0x0F,
            METATABLE_FIELDLAYOUT               = 0x10,
            METATABLE_STANDALONESIG             = 0x11,
            METATABLE_EVENTMAP                  = 0x12,
            METATABLE_EVENT                     = 0x14,
            METATABLE_PROPERTYMAP               = 0x15,
            METATABLE_PROPERTY                  = 0x17,
            METATABLE_METHODSEMANTICS           = 0x18,
            METATABLE_METHODIMPL                = 0x19,
            METATABLE_MODULEREF                 = 0x1A,
            METATABLE_TYPESPEC                  = 0x1B,
            METATABLE_IMPLMAP                   = 0x1C,
            METATABLE_FIELDRVA                  = 0x1D,
            METATABLE_ASSEMBLY                  = 0x20,
            METATABLE_ASSEMBLYPROCESSOR         = 0x21,
            METATABLE_ASSEMBLYOS                = 0x22,
            METATABLE_ASSEMBLYREF               = 0x23,
            METATABLE_ASSEMBLYREFPROCESSOR      = 0x24,
            METATABLE_ASSEMBLYREFOS             = 0x25,
            METATABLE_FILE                      = 0x26,
            METATABLE_EXPORTEDTYPE              = 0x27,
            METATABLE_MANIFESTRESOURCE          = 0x28,
            METATABLE_NESTEDCLASS               = 0x29,
            METATABLE_GENERICPARAM              = 0x2A,
            METATABLE_METHODSPEC                = 0x2B,
            METATABLE_GENERICPARAMCONSTRAINT    = 0x2C,

            NUM_META_TABLES,

            METATABLE_NONE                      = ~0
        };
        virtual eType           GetType                         () const = 0;

    protected:
        TableLayer*             m_pTableLayer;
        byte*                   m_pRecords;
        int                     m_iNumRecords;
        int                     m_iRecordSize;
    };

    template < class DerivedTable >
    class MetaRecordBase
    {
    public:
                                MetaRecordBase          () : m_pTable ( NULL ), m_pRecord ( NULL ) {}
                                MetaRecordBase          ( DerivedTable* pTable, void* pRecord ) : m_pTable ( pTable ), m_pRecord ( pRecord ) {}

        DerivedTable*           GetTable                () const    { return m_pTable; }

        dword                   GetIndex                () const
        {
            return 1 + ( (byte *)m_pRecord - (byte *)m_pTable->GetRecordsBase () ) / m_pTable->GetRecordSize ();
        }

    protected:
        DerivedTable*           m_pTable;
        void*                   m_pRecord;
    };

    template < class DerivedTable, class DerivedRecord >
    class MetaTableBase : public MetaTable
    {
    public:
        typedef DerivedRecord Record;

        virtual                     ~MetaTableBase      () {}

        Record                      GetRecord           ( dword dwIndex ) const
        {
            assert ( dwIndex >= 1 && dwIndex <= GetNumRecords () );

            void* pRecord = (byte *)m_pRecords + (dwIndex - 1)*GetRecordSize ();
            return Record ( (DerivedTable *)this, pRecord );
        }
    };

}
