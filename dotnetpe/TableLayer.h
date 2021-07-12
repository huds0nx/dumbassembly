#pragma once

namespace dotnetpe { class TableLayer; }

#include "TableLayer/MetaTable.h"
#include "TableLayer/MetaFieldReader.h"

#include "TableLayer/metafieldreaders/SimpleMetaFieldReader.h"
#include "TableLayer/metafieldreaders/StreamIndexMetaFieldReader.h"
#include "TableLayer/metafieldreaders/TableIndexMetaFieldReader.h"

#include "TableLayer/metatables/ModuleMetaTable.h"
#include "TableLayer/metatables/TypeRefMetaTable.h"
#include "TableLayer/metatables/TypeDefMetaTable.h"
#include "TableLayer/metatables/FieldMetaTable.h"
#include "TableLayer/metatables/MethodDefMetaTable.h"
#include "TableLayer/metatables/ParamMetaTable.h"
#include "TableLayer/metatables/InterfaceImplMetaTable.h"
#include "TableLayer/metatables/MemberRefMetaTable.h"
#include "TableLayer/metatables/ConstantMetaTable.h"
#include "TableLayer/metatables/CustomAttributeMetaTable.h"
#include "TableLayer/metatables/FieldMarshalMetaTable.h"
#include "TableLayer/metatables/DeclSecurityMetaTable.h"
#include "TableLayer/metatables/ClassLayoutMetaTable.h"
#include "TableLayer/metatables/FieldLayoutMetaTable.h"
#include "TableLayer/metatables/StandAloneSigMetaTable.h"
#include "TableLayer/metatables/EventMapMetaTable.h"
#include "TableLayer/metatables/EventMetaTable.h"
#include "TableLayer/metatables/PropertyMapMetaTable.h"
#include "TableLayer/metatables/PropertyMetaTable.h"
#include "TableLayer/metatables/MethodSemanticsMetaTable.h"
#include "TableLayer/metatables/MethodImplMetaTable.h"
#include "TableLayer/metatables/ModuleRefMetaTable.h"
#include "TableLayer/metatables/TypeSpecMetaTable.h"
#include "TableLayer/metatables/ImplMapMetaTable.h"
#include "TableLayer/metatables/FieldRVAMetaTable.h"
#include "TableLayer/metatables/AssemblyMetaTable.h"
#include "TableLayer/metatables/AssemblyProcessorMetaTable.h"
#include "TableLayer/metatables/AssemblyOSMetaTable.h"
#include "TableLayer/metatables/AssemblyRefMetaTable.h"
#include "TableLayer/metatables/AssemblyRefProcessorMetaTable.h"
#include "TableLayer/metatables/AssemblyRefOSMetaTable.h"
#include "TableLayer/metatables/FileMetaTable.h"
#include "TableLayer/metatables/ExportedTypeMetaTable.h"
#include "TableLayer/metatables/ManifestResourceMetaTable.h"
#include "TableLayer/metatables/NestedClassMetaTable.h"
#include "TableLayer/metatables/GenericParamMetaTable.h"
#include "TableLayer/metatables/MethodSpecMetaTable.h"
#include "TableLayer/metatables/GenericParamConstraintMetaTable.h"

namespace dotnetpe
{

    class DotNetPE;

    class TableLayer
    {
        friend DotNetPE;

    public:
                                        TableLayer          ( DotNetPE* pPE );
                                        ~TableLayer         ();

        void                            Init                ();
        DotNetPE*                       GetPE               () const    { return m_pPE; }

        byte                            GetHeapOffsetSizes  () const    { return m_ucHeapOffsetSizes; }

        MetaTable*                      GetMetaTable        ( MetaTable::eType type ) const;

    private:
        DotNetPE*                       m_pPE;

        byte                            m_ucHeapOffsetSizes;

        MetaTable*                      m_pMetaTables[MetaTable::NUM_META_TABLES];

        ModuleMetaTable                 m_ModuleTable;
        TypeRefMetaTable                m_TypeRefTable;
        TypeDefMetaTable                m_TypeDefTable;
        FieldMetaTable                  m_FieldTable;
        MethodDefMetaTable              m_MethodDefTable;
        ParamMetaTable                  m_ParamTable;
        InterfaceImplMetaTable          m_InterfaceImplTable;
        MemberRefMetaTable              m_MemberRefTable;
        ConstantMetaTable               m_ConstantTable;
        CustomAttributeMetaTable        m_CustomAttributeTable;
        FieldMarshalMetaTable           m_FieldMarshalTable;
        DeclSecurityMetaTable           m_DeclSecurityTable;
        ClassLayoutMetaTable            m_ClassLayoutTable;
        FieldLayoutMetaTable            m_FieldLayoutTable;
        StandAloneSigMetaTable          m_StandAloneSigTable;
        EventMapMetaTable               m_EventMapTable;
        EventMetaTable                  m_EventTable;
        PropertyMapMetaTable            m_PropertyMapTable;
        PropertyMetaTable               m_PropertyTable;
        MethodSemanticsMetaTable        m_MethodSemanticsTable;
        MethodImplMetaTable             m_MethodImplTable;
        ModuleRefMetaTable              m_ModuleRefTable;
        TypeSpecMetaTable               m_TypeSpecTable;
        ImplMapMetaTable                m_ImplMapTable;
        FieldRVAMetaTable               m_FieldRVATable;
        AssemblyMetaTable               m_AssemblyTable;
        AssemblyProcessorMetaTable      m_AssemblyProcessorTable;
        AssemblyOSMetaTable             m_AssemblyOSTable;
        AssemblyRefMetaTable            m_AssemblyRefTable;
        AssemblyRefProcessorMetaTable   m_AssemblyRefProcessorTable;
        AssemblyRefOSMetaTable          m_AssemblyRefOSTable;
        FileMetaTable                   m_FileTable;
        ExportedTypeMetaTable           m_ExportedTypeTable;
        ManifestResourceMetaTable       m_ManifestResourceTable;
        NestedClassMetaTable            m_NestedClassTable;
        GenericParamMetaTable           m_GenericParamTable;
        MethodSpecMetaTable             m_MethodSpecTable;
        GenericParamConstraintMetaTable m_GenericParamConstraintTable;
    };

}
