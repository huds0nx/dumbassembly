#include "StdInc.h"

namespace dotnetpe
{

    TableLayer::TableLayer ( DotNetPE* pPE )
        : m_pPE ( pPE )
    {
        memset ( &m_pMetaTables, 0, sizeof(m_pMetaTables) );

        m_pMetaTables[MetaTable::METATABLE_MODULE] = &m_ModuleTable;
        m_pMetaTables[MetaTable::METATABLE_TYPEREF] = &m_TypeRefTable;
        m_pMetaTables[MetaTable::METATABLE_TYPEDEF] = &m_TypeDefTable;
        m_pMetaTables[MetaTable::METATABLE_FIELD] = &m_FieldTable;
        m_pMetaTables[MetaTable::METATABLE_METHODDEF] = &m_MethodDefTable;
        m_pMetaTables[MetaTable::METATABLE_PARAM] = &m_ParamTable;
        m_pMetaTables[MetaTable::METATABLE_INTERFACEIMPL] = &m_InterfaceImplTable;
        m_pMetaTables[MetaTable::METATABLE_MEMBERREF] = &m_MemberRefTable;
        m_pMetaTables[MetaTable::METATABLE_CONSTANT] = &m_ConstantTable;
        m_pMetaTables[MetaTable::METATABLE_CUSTOMATTRIBUTE] = &m_CustomAttributeTable;
        m_pMetaTables[MetaTable::METATABLE_FIELDMARSHAL] = &m_FieldMarshalTable;
        m_pMetaTables[MetaTable::METATABLE_DECLSECURITY] = &m_DeclSecurityTable;
        m_pMetaTables[MetaTable::METATABLE_CLASSLAYOUT] = &m_ClassLayoutTable;
        m_pMetaTables[MetaTable::METATABLE_FIELDLAYOUT] = &m_FieldLayoutTable;
        m_pMetaTables[MetaTable::METATABLE_STANDALONESIG] = &m_StandAloneSigTable;
        m_pMetaTables[MetaTable::METATABLE_EVENTMAP] = &m_EventMapTable;
        m_pMetaTables[MetaTable::METATABLE_EVENT] = &m_EventTable;
        m_pMetaTables[MetaTable::METATABLE_PROPERTYMAP] = &m_PropertyMapTable;
        m_pMetaTables[MetaTable::METATABLE_PROPERTY] = &m_PropertyTable;
        m_pMetaTables[MetaTable::METATABLE_METHODSEMANTICS] = &m_MethodSemanticsTable;
        m_pMetaTables[MetaTable::METATABLE_METHODIMPL] = &m_MethodImplTable;
        m_pMetaTables[MetaTable::METATABLE_MODULEREF] = &m_ModuleRefTable;
        m_pMetaTables[MetaTable::METATABLE_TYPESPEC] = &m_TypeSpecTable;
        m_pMetaTables[MetaTable::METATABLE_IMPLMAP] = &m_ImplMapTable;
        m_pMetaTables[MetaTable::METATABLE_FIELDRVA] = &m_FieldRVATable;
        m_pMetaTables[MetaTable::METATABLE_ASSEMBLY] = &m_AssemblyTable;
        m_pMetaTables[MetaTable::METATABLE_ASSEMBLYPROCESSOR] = &m_AssemblyProcessorTable;
        m_pMetaTables[MetaTable::METATABLE_ASSEMBLYOS] = &m_AssemblyOSTable;
        m_pMetaTables[MetaTable::METATABLE_ASSEMBLYREF] = &m_AssemblyRefTable;
        m_pMetaTables[MetaTable::METATABLE_ASSEMBLYREFPROCESSOR] = &m_AssemblyRefProcessorTable;
        m_pMetaTables[MetaTable::METATABLE_ASSEMBLYREFOS] = &m_AssemblyRefOSTable;
        m_pMetaTables[MetaTable::METATABLE_FILE] = &m_FileTable;
        m_pMetaTables[MetaTable::METATABLE_EXPORTEDTYPE] = &m_ExportedTypeTable;
        m_pMetaTables[MetaTable::METATABLE_MANIFESTRESOURCE] = &m_ManifestResourceTable;
        m_pMetaTables[MetaTable::METATABLE_NESTEDCLASS] = &m_NestedClassTable;
        m_pMetaTables[MetaTable::METATABLE_GENERICPARAM] = &m_GenericParamTable;
        m_pMetaTables[MetaTable::METATABLE_METHODSPEC] = &m_MethodSpecTable;
        m_pMetaTables[MetaTable::METATABLE_GENERICPARAMCONSTRAINT] = &m_GenericParamConstraintTable;
    }

    TableLayer::~TableLayer ()
    {
        
    }

    void TableLayer::Init ()
    {
        MetaStream* pTableStream = m_pPE->GetStreamLayer ()->GetTableMetaStream ();
        MemoryStream metaData ( pTableStream->Data (), pTableStream->Size (), true );
        dword dwReserved = metaData.ReadUInt32 ();
        byte ucMajorVersion = metaData.ReadByte ();
        byte ucMinorVersion = metaData.ReadByte ();
        m_ucHeapOffsetSizes = metaData.ReadByte ();
        byte ucReserved = metaData.ReadByte ();

        unsigned __int64 ullValid = metaData.ReadUInt64 ();
        unsigned __int64 ullSorted = metaData.ReadUInt64 ();

        // Set up the metatables
        for ( int i = 0; i < MetaTable::NUM_META_TABLES; i++ )
        {
            if ( ullValid & 1 )
            {
                dword dwNumRecords = metaData.ReadUInt32 ();
                if ( m_pMetaTables[i] )
                    m_pMetaTables[i]->SetNumRecords ( dwNumRecords );
                else
                    _asm int 3
            }

            ullValid >>= 1;
        }

        for ( int i = 0; i < MetaTable::NUM_META_TABLES; i++ )
        {
            if ( m_pMetaTables[i] )
                m_pMetaTables[i]->Init ( this );
        }

        byte* pRecords = (byte *)metaData.Data () + metaData.GetPosition ();
        for ( int i = 0; i < MetaTable::NUM_META_TABLES; i++ )
        {
            if ( m_pMetaTables[i] )
            {
                m_pMetaTables[i]->SetRecordsBase ( pRecords );
                pRecords += m_pMetaTables[i]->GetNumRecords () * m_pMetaTables[i]->GetRecordSize ();
            }
        }
    }

    MetaTable* TableLayer::GetMetaTable ( MetaTable::eType type ) const
    {
        if ( type >= MetaTable::NUM_META_TABLES || type == MetaTable::METATABLE_NONE )
            return NULL;

        return m_pMetaTables [ type ];
    }

}
